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

void vkShaderGraphGL4::GenerateShadow(vkSGShaderGraph *graph, unsigned numLayers)
{
  std::set<vkSGOutput*> outputs;
  std::set<vkSGOutput*> preAlphaOutputs;
  std::set<vkSGOutput*> postAlphaOutputs;

  vkSGOutput *alphaOutput = graph->GetAlpha();
  if (alphaOutput)
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

  vkShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  ctx.EvaluateInlines(outputs);

  std::set<vkShaderGraphCtx::ExternalBinding> bindings = ctx.GetBindingsFor(outputs);

  ctx.GenerateCode(preAlphaOutputs);
  vkString preAlphaCode = ctx.GetCode();


  bool vsm = false;

  std::ostringstream ss;

  // generate the vertex shader
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "uniform mat4 vk_MatModel;" << std::endl
    << std::endl
    << "in vec4 vk_Position;" << std::endl
    << "in vec4 vk_TexCoord0;" << std::endl
    << std::endl
    // if we use hardware skinning this should be placed here
    << "out vec2 inGeomTexCoord;" << std::endl
    << std::endl;
  ss << "void main()" << std::endl
    << "{" << std::endl
    << "  gl_Position = vk_MatModel * vk_Position;" << std::endl
    << "  inGeomTexCoord = vk_TexCoord0;" << std::endl
    << "}" << std::endl
    << std::endl;
  vkString vertexShaderSources = ss.str();

  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "layout(triangles) in;" << std::endl
    << "layout(triangle_strip, max_vertices = " << (numLayers * 3) << ") out;" << std::endl
    << "uniform mat4 vk_ShadowMapMatProjView[" << numLayers << "];" << std::endl
    << "in vec2 inGeomTexCoord[];" << std::endl
    << std::endl
    << "out vec2 inFragTexCoord;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl;
  
  for (unsigned i = 0; i < numLayers; ++i)
  {
    ss << "  gl_Layer = " << i << ";" << std::endl
      << "  gl_Position = vk_ShadowMapMatProjView[" << i << "] * gl_in[0].gl_Position;" << std::endl
      << "  inFragTexCoord = inGeomTexCoord[0];" << std::endl
      << "  EmitVertex();" << std::endl
      << "  " << std::endl
      << "  gl_Layer = " << i << ";" << std::endl
      << "  gl_Position = vk_ShadowMapMatProjView[" << i << "] * gl_in[1].gl_Position;" << std::endl
      << "  inFragTexCoord = inGeomTexCoord[1];" << std::endl
      << "  EmitVertex();" << std::endl
      << "  " << std::endl
      << "  gl_Layer = " << i << ";" << std::endl
      << "  gl_Position = vk_ShadowMapMatProjView[" << i << "] * gl_in[2].gl_Position;" << std::endl
      << "  inFragTexCoord = inGeomTexCoord[2];" << std::endl
      << "  EmitVertex();" << std::endl
      << "  " << std::endl
      << "  EndPrimitive();" << std::endl
  }




  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "layout(location = 0) out vec4 vk_FragColor;" << std::endl
    << std::endl;
  for (const vkShaderGraphCtx::ExternalBinding &binding : bindings)
  {
    ss << "uniform " << binding.variableType << " " << binding.variableName << ";" << std::endl;
  }
  ss << std::endl
    << "in vec2 inFragTexCoord;" << std::endl
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
  if (vsm)
  {

  }
  else
  {
    ss << "  vk_FragColor = vec4(0, 0, 0, 0);" << std::endl;
  }
  // if VSM this can be 
  ss << "}" << std::endl
    << std::endl;
  vkString fragmentShaderSources = ss.str();

  ss.clear();


  vkShaderGL4 *vertexShader = new vkShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexShaderSources);
  if (!vertexShader->Compile())
  {
    vertexShader->Release();
    return;
  }

  vkShaderGL4 *fragmentShader = new vkShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentShaderSources);
  if (!fragmentShader->Compile())
  {
    vertexShader->Release();
    fragmentShader->Release();
    return;
  }

  vkProgramGL4 *gBufferShader = new vkProgramGL4();
  gBufferShader->AttachShader(vertexShader);
  gBufferShader->AttachShader(fragmentShader);
  if (!gBufferShader->Link())
  {
    vertexShader->Release();
    fragmentShader->Release();
    gBufferShader->Release();
    return;
  }


  graph->SetShader(eRP_GBuffer, gBufferShader);

}
