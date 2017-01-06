#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.hh>
#include <GraphicsGL4/gl4program.hh>
#include <GraphicsGL4/gl4shader.hh>
#include <iostream>
#include <sstream>

static const char *compareMode[] = {
  "<=",
  ">=",
  "<",
  ">",
  "==",
  "!="
};

void vkShaderGraphGL4::GenerateGBuffer(vkSGShaderGraph *graph)
{
  graph->SetShader(eRP_GBuffer, 0);

  std::set<vkSGOutput*> outputs;
  std::set<vkSGOutput*> preAlphaOutputs;
  std::set<vkSGOutput*> postAlphaOutputs;

  vkSGOutput *diffuseOutput = graph->GetDiffuse();
  vkSGOutput *normalOutput = graph->GetNormal();
  vkSGOutput *roughnessOutput = graph->GetRoughness();
  vkSGOutput *alphaOutput = graph->GetAlpha();
  if (!diffuseOutput)
  {
    printf("No diffuse output assigned\n");
    return;
  }
  if (diffuseOutput->GetDataType() == eSGDT_Float4)
  {
    vkSGSplitFloat4 *diffuseSplit = new vkSGSplitFloat4();
    diffuseSplit->GetInput(0)->SetInput(diffuseOutput);

    vkSGFloat3 *vec3 = new vkSGFloat3();
    vec3->SetInput(0, diffuseSplit, 0);
    vec3->SetInput(1, diffuseSplit, 1);
    vec3->SetInput(2, diffuseSplit, 2);

    diffuseOutput = vec3->GetOutput(0);
  }
  else if (diffuseOutput->GetDataType() == eSGDT_Float2)
  {
    printf("Invalid input type for diffuse\n");
    return;
  }
  else if (diffuseOutput->GetDataType() == eSGDT_Float)
  {
    vkSGFloat3 *vec3 = new vkSGFloat3();
    vec3->GetInput(0)->SetInput(diffuseOutput);
    vec3->GetInput(1)->SetInput(diffuseOutput);
    vec3->GetInput(2)->SetInput(diffuseOutput);

    diffuseOutput = vec3->GetOutput(0);
  }
  if (normalOutput)
  {
    if (normalOutput->GetDataType() == eSGDT_Float4)
    {
      vkSGSplitFloat4 *normalSplit = new vkSGSplitFloat4();
      normalSplit->GetInput(0)->SetInput(normalOutput);

      vkSGFloat3 *vec3 = new vkSGFloat3();
      vec3->SetInput(0, normalSplit, 0);
      vec3->SetInput(1, normalSplit, 1);
      vec3->SetInput(2, normalSplit, 2);

      normalOutput = vec3->GetOutput(0);
    }
    else if (normalOutput->GetDataType() == eSGDT_Float2)
    {
      printf("Invalid input type for diffuse\n");
      return;
    }
    else if (normalOutput->GetDataType() == eSGDT_Float)
    {
      vkSGFloat3 *vec3 = new vkSGFloat3();
      vec3->GetInput(0)->SetInput(normalOutput);
      vec3->GetInput(1)->SetInput(normalOutput);
      vec3->GetInput(2)->SetInput(normalOutput);

      normalOutput = vec3->GetOutput(0);
    }
  }
  if (roughnessOutput)
  {
    if (roughnessOutput->GetDataType() != eSGDT_Float)
    {
      roughnessOutput = 0;
    }
    else
    {
      outputs.insert(roughnessOutput);
      postAlphaOutputs.insert(roughnessOutput);
    }
  }
  if (graph->IsDiscardAlpha() && alphaOutput)
  {
    if (alphaOutput->GetDataType() != eSGDT_Float)
    {
      alphaOutput = 0;
    }
    else
    {
      outputs.insert(alphaOutput);
      preAlphaOutputs.insert(alphaOutput);
    }
  }
  if (normalOutput)
  {
    outputs.insert(normalOutput);
    postAlphaOutputs.insert(normalOutput);
  }

  outputs.insert(diffuseOutput);
  postAlphaOutputs.insert(diffuseOutput);

  vkShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  ctx.EvaluateInlines(outputs);


  ctx.GenerateCode(preAlphaOutputs);
  vkString preAlphaCode = ctx.GetCode();

  ctx.GenerateCode(postAlphaOutputs);
  vkString postAlphaCode = ctx.GetCode();

  ctx.GenerateCode(outputs);
  std::set<vkShaderGraphCtx::ExternalBinding> bindings = ctx.GetBindingsFor(outputs);

  std::ostringstream ss;

  // generate the vertex shader
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "uniform mat4 vk_MatProjViewModel;" << std::endl
    << "uniform mat4 vk_MatModel;" << std::endl
    << std::endl
    << "in vec4 vk_Position;" << std::endl
    << "in vec3 vk_Normal;" << std::endl
    << "in vec3 vk_Tangent;" << std::endl
    << "in vec3 vk_BiNormal;" << std::endl
    << "in vec2 vk_TexCoord0;" << std::endl
    << std::endl;
  if (graph->IsSkinnedMaterial())
  {
    ss // add all the bindings that are needed to correctly perform multi bone skinning
      << "uniform mat4 vk_MatsSkeleton[" << graph->GetMaxBones() << "];" << std::endl
      << "uniform int vk_SkeletonMapping[" << graph->GetMaxBones() << "];" << std::endl
      << "in ivec4 vk_BoneIndex;" << std::endl
      << "in vec4 vk_BoneWeight;" << std::endl;
  }

  ss // if we use hardware skinning this should be placed here
    << "out vec2 inFragTexCoord;" << std::endl
    << "out vec3 inFragNormal;" << std::endl
    << "out vec3 inFragTangent;" << std::endl
    << "out vec3 inFragBiNormal;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl
    << "  mat3 matModel3 = mat3(vk_MatModel); " << std::endl;
  if (graph->IsSkinnedMaterial())
  {
    ss
      << "  int idx0 = vk_SkeletonMapping[vk_BoneIndex.x];" << std::endl
      << "  int idx1 = vk_SkeletonMapping[vk_BoneIndex.y];" << std::endl
      << "  int idx2 = vk_SkeletonMapping[vk_BoneIndex.z];" << std::endl
      << "  int idx3 = vk_SkeletonMapping[vk_BoneIndex.w];" << std::endl
      << "  mat4 boneMat40 = vk_MatsSkeleton[idx0];" << std::endl
      << "  mat4 boneMat41 = vk_MatsSkeleton[idx1];" << std::endl
      << "  mat4 boneMat42 = vk_MatsSkeleton[idx2];" << std::endl
      << "  mat4 boneMat43 = vk_MatsSkeleton[idx3];" << std::endl
      << "  mat3 boneMat30 = mat3(boneMat40); " << std::endl
      << "  mat3 boneMat31 = mat3(boneMat41); " << std::endl
      << "  mat3 boneMat32 = mat3(boneMat42); " << std::endl
      << "  mat3 boneMat33 = mat3(boneMat43); " << std::endl
      << "  vec4 position = boneMat40 * vk_Position * vk_BoneWeight.x + " << std::endl
      << "                  boneMat41 * vk_Position * vk_BoneWeight.y + " << std::endl
      << "                  boneMat42 * vk_Position * vk_BoneWeight.z + " << std::endl
      << "                  boneMat43 * vk_Position * vk_BoneWeight.w;" << std::endl
      << "  vec3 normal = boneMat30 * vk_Normal * vk_BoneWeight.x + " << std::endl
      << "                boneMat31 * vk_Normal * vk_BoneWeight.y + " << std::endl
      << "                boneMat32 * vk_Normal * vk_BoneWeight.z + " << std::endl
      << "                boneMat33 * vk_Normal * vk_BoneWeight.w;" << std::endl
      << "  vec3 tangent = boneMat30 * vk_Tangent * vk_BoneWeight.x + " << std::endl
      << "                 boneMat31 * vk_Tangent * vk_BoneWeight.y + " << std::endl
      << "                 boneMat32 * vk_Tangent * vk_BoneWeight.z + " << std::endl
      << "                 boneMat33 * vk_Tangent * vk_BoneWeight.w;" << std::endl
      << "  vec3 binormal = boneMat30 * vk_BiNormal * vk_BoneWeight.x + " << std::endl
      << "                  boneMat31 * vk_BiNormal * vk_BoneWeight.y + " << std::endl
      << "                  boneMat32 * vk_BiNormal * vk_BoneWeight.z + " << std::endl
      << "                  boneMat33 * vk_BiNormal * vk_BoneWeight.w;" << std::endl
      << "  gl_Position = vk_MatProjViewModel * position;" << std::endl
      << "  inFragNormal = matModel3 * normal;" << std::endl
      << "  inFragTangent = matModel3 * tangent;" << std::endl
      << "  inFragBiNormal = matModel3 * binormal;" << std::endl;

  }
  else
  {

    ss
      << "  gl_Position = vk_MatProjViewModel * vk_Position;" << std::endl
      << "  inFragNormal = matModel3 * vk_Normal;" << std::endl
      << "  inFragTangent = matModel3 * vk_Tangent;" << std::endl
      << "  inFragBiNormal = matModel3 * vk_BiNormal;" << std::endl;
  }
  ss 
    << "  inFragTexCoord = vk_TexCoord0;" << std::endl
    << "}" << std::endl
    << std::endl;
  vkString vertexShaderSources = ss.str();

  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "layout(location = 0) out vec4 vk_DiffuseRoughness;" << std::endl
    << "layout(location = 1) out vec4 vk_NormalLightMode;" << std::endl
    << "layout(location = 2) out vec4 vk_EmissivMetallic;" << std::endl
    << "layout(location = 3) out vec4 vk_SSSSpecular;" << std::endl
    << std::endl;
  for (const vkShaderGraphCtx::ExternalBinding &binding : bindings)
  {
    ss << "uniform " << binding.variableType << " " << binding.variableName << ";" << std::endl;
  }
  
  ss << std::endl
    << "in vec2 inFragTexCoord;" << std::endl
    << "in vec3 inFragNormal;" << std::endl
    << "in vec3 inFragTangent;" << std::endl
    << "in vec3 inFragBiNormal;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl;
  if (graph->IsDiscardAlpha() && alphaOutput)
  {

    ss << preAlphaCode << std::endl
      << "  if (" << ctx.GetFullOutputValue(alphaOutput) << " " << compareMode[graph->GetDiscardAlphaCompareMode()] << " " << std::to_string(graph->GetDiscardAlphaThreshold()) << ")" << std::endl
      << "  {" << std::endl
      << "    discard;" << std::endl
      << "  }" << std::endl;
  }
  ss << postAlphaCode << std::endl;
  vkString roughness = "0.0";
  if (roughnessOutput)
  {
    roughness = ctx.GetFullOutputValue(roughnessOutput);
  }
  vkString normal = "vec3(0, 0, 1)";
  if (normalOutput)
  {
    normal = "((" + ctx.GetFullOutputValue(normalOutput) + ") * 2.0 - 1.0)";
  }

  ss << "  vec3 n = normalize(inFragNormal);" << std::endl
    << "  vec3 t = normalize(inFragTangent);" << std::endl
    << "  vec3 b = normalize(inFragBiNormal);" << std::endl
    << "  mat3 faceToModel = mat3(t, b, n);" << std::endl
    << "  vk_DiffuseRoughness = vec4(" << ctx.GetFullOutputValue(diffuseOutput) << ", " << roughness << ");" << std::endl
    << "  vk_NormalLightMode = vec4((faceToModel * " << normal << ") * 0.5 + 0.5, 0.0);" << std::endl
    << "  vk_EmissivMetallic = vec4(0, 0, 0, 0);" << std::endl
    << "  vk_SSSSpecular = vec4(0, 0, 0, 0);" << std::endl
    << "}" << std::endl
    << std::endl;
  vkString fragmentShaderSources = ss.str();

  ss.clear();


  vkShaderGL4 *vertexShader = new vkShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexShaderSources);
  if (!vertexShader->Compile())
  {
    DebugCode("GBuffer.Vertex", vertexShaderSources.c_str());
    printf("Unable to compile vertex shader:\n%s\n", vertexShader->GetCompileErrorLog().c_str());
    vertexShader->Release();
    return;
  }

  vkShaderGL4 *fragmentShader = new vkShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentShaderSources);
  if (!fragmentShader->Compile())
  {
    DebugCode("GBuffer.Fragment", fragmentShaderSources.c_str());
    printf("Unable to compile vertex shader:\n%s\n", fragmentShader->GetCompileErrorLog().c_str());
    vertexShader->Release();
    fragmentShader->Release();
    return;
  }

  vkProgramGL4 *gBufferShader = new vkProgramGL4();
  gBufferShader->AttachShader(vertexShader);
  gBufferShader->AttachShader(fragmentShader);
  if (!gBufferShader->Link())
  {
    DebugCode("GBuffer.Vertex", vertexShaderSources.c_str());
    DebugCode("GBuffer.Fragment", fragmentShaderSources.c_str());
    printf("Unable to link shader:\n%s\n", gBufferShader->GetLinkErrorLog().c_str());
    vertexShader->Release();
    fragmentShader->Release();
    gBufferShader->Release();
    return;
  }


  graph->SetShader(eRP_GBuffer, gBufferShader);

  vertexShader->Release();
  fragmentShader->Release();
  gBufferShader->Release();

  fflush(stdout);
}
