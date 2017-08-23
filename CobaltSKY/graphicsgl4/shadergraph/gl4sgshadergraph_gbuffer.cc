#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4shader.hh>
#include <cobalt/graphics/shadergraph/cssgfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.hh>
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

void csShaderGraphGL4::GenerateGBuffer(csSGShaderGraph *graph, iSGShaderGraphLogger *logger)
{
  graph->SetShader(eRP_GBuffer, 0);

  std::set<csSGOutput*> outputs;
  std::set<csSGOutput*> preAlphaOutputs;
  std::set<csSGOutput*> postAlphaOutputs;

  csSGOutput *diffuseOutput = graph->GetDiffuse();
  csSGOutput *normalOutput = graph->GetNormal();
  csSGOutput *roughnessOutput = graph->GetRoughness();
  csSGOutput *alphaOutput = graph->GetAlpha();
  if (!diffuseOutput)
  {
    if (logger)
    {
      logger->LogInfo("GBuffer", "No diffuse output assigned");
    }
    return;
  }
  if (diffuseOutput->GetDataType() == eSGDT_Float4)
  {
    csSGSplitFloat4 *diffuseSplit = new csSGSplitFloat4();
    diffuseSplit->GetInput(0)->SetInput(diffuseOutput);

    csSGFloat3 *vec3 = new csSGFloat3();
    vec3->SetInput(0, diffuseSplit, 0);
    vec3->SetInput(1, diffuseSplit, 1);
    vec3->SetInput(2, diffuseSplit, 2);

    diffuseOutput = vec3->GetOutput(0);
  }
  else if (diffuseOutput->GetDataType() == eSGDT_Float2)
  {
    logger->LogInfo("GBuffer", "Invalid input type for diffuse");
    return;
  }
  else if (diffuseOutput->GetDataType() == eSGDT_Float)
  {
    csSGFloat3 *vec3 = new csSGFloat3();
    vec3->GetInput(0)->SetInput(diffuseOutput);
    vec3->GetInput(1)->SetInput(diffuseOutput);
    vec3->GetInput(2)->SetInput(diffuseOutput);

    diffuseOutput = vec3->GetOutput(0);
  }
  if (normalOutput)
  {
    if (normalOutput->GetDataType() == eSGDT_Float4)
    {
      csSGSplitFloat4 *normalSplit = new csSGSplitFloat4();
      normalSplit->GetInput(0)->SetInput(normalOutput);

      csSGFloat3 *vec3 = new csSGFloat3();
      vec3->SetInput(0, normalSplit, 0);
      vec3->SetInput(1, normalSplit, 1);
      vec3->SetInput(2, normalSplit, 2);

      normalOutput = vec3->GetOutput(0);
    }
    else if (normalOutput->GetDataType() == eSGDT_Float2)
    {
      logger->LogInfo("GBuffer", "Invalid input type for diffuse");
      return;
    }
    else if (normalOutput->GetDataType() == eSGDT_Float)
    {
      csSGFloat3 *vec3 = new csSGFloat3();
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

  csShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  ctx.EvaluateInlines(outputs);


  ctx.GenerateCode(preAlphaOutputs);
  std::string preAlphaCode = ctx.GetCode();

  ctx.GenerateCode(postAlphaOutputs);
  std::string postAlphaCode = ctx.GetCode();

  ctx.GenerateCode(outputs);
  std::set<csShaderGraphCtx::ExternalBinding> bindings = ctx.GetBindingsFor(outputs);

  std::ostringstream ss;

  // generate the vertex shader
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "uniform mat4 cs_MatProjViewModel;" << std::endl
    << "uniform mat4 cs_MatModel;" << std::endl
    << std::endl
    << "in vec4 cs_Position;" << std::endl
    << "in vec3 cs_Normal;" << std::endl
    << "in vec3 cs_Tangent;" << std::endl
    << "in vec3 cs_BiNormal;" << std::endl
    << "in vec2 cs_TexCoord0;" << std::endl
    << std::endl;
  if (graph->IsSkinnedMaterial())
  {
    ss // add all the bindings that are needed to correctly perform multi bone skinning
      << "uniform mat4 cs_MatsSkeleton[" << graph->GetMaxBones() << "];" << std::endl
      << "uniform int cs_SkeletonMapping[" << graph->GetMaxBones() << "];" << std::endl
      << "in ivec4 cs_BoneIndex;" << std::endl
      << "in vec4 cs_BoneWeight;" << std::endl;
  }

  ss // if we use hardware skinning this should be placed here
    << "out vec2 inFragTexCoord;" << std::endl
    << "out vec3 inFragNormal;" << std::endl
    << "out vec3 inFragTangent;" << std::endl
    << "out vec3 inFragBiNormal;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl
    << "  mat3 matModel3 = mat3(cs_MatModel); " << std::endl;
  if (graph->IsSkinnedMaterial())
  {
    ss
      << "  int idx0 = cs_SkeletonMapping[cs_BoneIndex.x];" << std::endl
      << "  int idx1 = cs_SkeletonMapping[cs_BoneIndex.y];" << std::endl
      << "  int idx2 = cs_SkeletonMapping[cs_BoneIndex.z];" << std::endl
      << "  int idx3 = cs_SkeletonMapping[cs_BoneIndex.w];" << std::endl
      << "  mat4 boneMat40 = cs_MatsSkeleton[idx0];" << std::endl
      << "  mat4 boneMat41 = cs_MatsSkeleton[idx1];" << std::endl
      << "  mat4 boneMat42 = cs_MatsSkeleton[idx2];" << std::endl
      << "  mat4 boneMat43 = cs_MatsSkeleton[idx3];" << std::endl
      << "  mat3 boneMat30 = mat3(boneMat40); " << std::endl
      << "  mat3 boneMat31 = mat3(boneMat41); " << std::endl
      << "  mat3 boneMat32 = mat3(boneMat42); " << std::endl
      << "  mat3 boneMat33 = mat3(boneMat43); " << std::endl
      << "  vec4 position = boneMat40 * cs_Position * cs_BoneWeight.x + " << std::endl
      << "                  boneMat41 * cs_Position * cs_BoneWeight.y + " << std::endl
      << "                  boneMat42 * cs_Position * cs_BoneWeight.z + " << std::endl
      << "                  boneMat43 * cs_Position * cs_BoneWeight.w;" << std::endl
      << "  vec3 normal = boneMat30 * cs_Normal * cs_BoneWeight.x + " << std::endl
      << "                boneMat31 * cs_Normal * cs_BoneWeight.y + " << std::endl
      << "                boneMat32 * cs_Normal * cs_BoneWeight.z + " << std::endl
      << "                boneMat33 * cs_Normal * cs_BoneWeight.w;" << std::endl
      << "  vec3 tangent = boneMat30 * cs_Tangent * cs_BoneWeight.x + " << std::endl
      << "                 boneMat31 * cs_Tangent * cs_BoneWeight.y + " << std::endl
      << "                 boneMat32 * cs_Tangent * cs_BoneWeight.z + " << std::endl
      << "                 boneMat33 * cs_Tangent * cs_BoneWeight.w;" << std::endl
      << "  vec3 binormal = boneMat30 * cs_BiNormal * cs_BoneWeight.x + " << std::endl
      << "                  boneMat31 * cs_BiNormal * cs_BoneWeight.y + " << std::endl
      << "                  boneMat32 * cs_BiNormal * cs_BoneWeight.z + " << std::endl
      << "                  boneMat33 * cs_BiNormal * cs_BoneWeight.w;" << std::endl
      << "  gl_Position = cs_MatProjViewModel * position;" << std::endl
      << "  inFragNormal = matModel3 * normal;" << std::endl
      << "  inFragTangent = matModel3 * tangent;" << std::endl
      << "  inFragBiNormal = matModel3 * binormal;" << std::endl;

  }
  else
  {

    ss
      << "  gl_Position = cs_MatProjViewModel * cs_Position;" << std::endl
      << "  inFragNormal = matModel3 * cs_Normal;" << std::endl
      << "  inFragTangent = matModel3 * cs_Tangent;" << std::endl
      << "  inFragBiNormal = matModel3 * cs_BiNormal;" << std::endl;
  }
  ss 
    << "  inFragTexCoord = cs_TexCoord0;" << std::endl
    << "}" << std::endl
    << std::endl;
  std::string vertexShaderSources = ss.str();
  if (logger)
  {
    logger->LogSourceCode("GBuffer", "VertexShader", vertexShaderSources);
  }
  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "layout(location = 0) out vec4 cs_DiffuseRoughness;" << std::endl
    << "layout(location = 1) out vec4 cs_NormalLightMode;" << std::endl
    << "layout(location = 2) out vec4 cs_EmissivMetallic;" << std::endl
    << "layout(location = 3) out vec4 cs_SSSSpecular;" << std::endl
    << std::endl;
  for (const csShaderGraphCtx::ExternalBinding &binding : bindings)
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
  std::string roughness = "0.0";
  if (roughnessOutput)
  {
    roughness = ctx.GetFullOutputValue(roughnessOutput);
  }
  std::string normal = "vec3(0, 0, 1)";
  if (normalOutput)
  {
    normal = "((" + ctx.GetFullOutputValue(normalOutput) + ") * 2.0 - 1.0)";
  }

  ss << "  vec3 n = normalize(inFragNormal);" << std::endl
    << "  vec3 t = normalize(inFragTangent);" << std::endl
    << "  vec3 b = normalize(inFragBiNormal);" << std::endl
    << "  mat3 faceToModel = mat3(t, b, n);" << std::endl
    << "  cs_DiffuseRoughness = vec4(" << ctx.GetFullOutputValue(diffuseOutput) << ", " << roughness << ");" << std::endl
    << "  cs_NormalLightMode = vec4((faceToModel * " << normal << ") * 0.5 + 0.5, 0.0);" << std::endl
    << "  cs_EmissivMetallic = vec4(0, 0, 0, 0);" << std::endl
    << "  cs_SSSSpecular = vec4(0, 0, 0, 0);" << std::endl
    << "}" << std::endl
    << std::endl;
  std::string fragmentShaderSources = ss.str();
  if (logger)
  {
    logger->LogSourceCode("GBuffer", "FragmentShader", fragmentShaderSources);
  }

  ss.clear();


  csShaderGL4 *vertexShader = new csShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexShaderSources);
  if (!vertexShader->Compile())
  {
    if (logger)
    {
      logger->LogError("GBuffer.Vertex", vertexShader->GetCompileErrorLog());
    }
    vertexShader->Release();
    return;
  }

  csShaderGL4 *fragmentShader = new csShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentShaderSources);
  if (!fragmentShader->Compile())
  {
    if (logger)
    {
      logger->LogError("GBuffer.Fragment", fragmentShader->GetCompileErrorLog());
    }

    vertexShader->Release();
    fragmentShader->Release();
    return;
  }

  csProgramGL4 *gBufferShader = new csProgramGL4();
  gBufferShader->AttachShader(vertexShader);
  gBufferShader->AttachShader(fragmentShader);
  if (!gBufferShader->Link())
  {
    if (logger)
    {
      logger->LogError("GBuffer.Link", gBufferShader->GetLinkErrorLog());
    }

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
