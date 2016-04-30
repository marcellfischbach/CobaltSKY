#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.hh>
#include <GraphicsGL4/Shader.hh>
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
    << "in vec2 vk_TexCoord0;" << std::endl
    << std::endl
    // if we use hardware skinning this should be placed here
    << "out vec2 inFragTexCoord;" << std::endl
    << "out vec3 inFragNormal;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl
    << "  gl_Position = vk_MatProjViewModel * vk_Position;" << std::endl
    << "  inFragTexCoord = vk_TexCoord0;" << std::endl
    << "  inFragNormal = mat3(vk_MatModel) * vk_Normal;" << std::endl
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
  ss << "  vk_DiffuseRoughness = vec4(" << ctx.GetFullOutputValue(diffuseOutput) << ", " << roughness << ");" << std::endl
    << "  vk_NormalLightMode = vec4(inFragNormal * 0.5 + 0.5, 0.0);" << std::endl
    << "  vk_EmissivMetallic = vec4(0, 0, 0, 0);" << std::endl
    << "  vk_SSSSpecular = vec4(0, 0, 0, 0);" << std::endl
    << "}" << std::endl
    << std::endl;
  vkString fragmentShaderSources = ss.str();

  ss.clear();

  DebugCode("GBuffer.Vertex", vertexShaderSources.c_str());
  DebugCode("GBuffer.Fragment", fragmentShaderSources.c_str());

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

}
