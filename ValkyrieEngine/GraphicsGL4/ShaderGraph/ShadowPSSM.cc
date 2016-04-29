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

void vkShaderGraphGL4::GenerateShadow(vkSGShaderGraph *graph, unsigned numLayers, vkRenderPass renderPass)
{
  graph->SetShader(renderPass, 0);

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

  ctx.GenerateCode(preAlphaOutputs);
  vkString preAlphaCode = ctx.GetCode();

  ctx.GenerateCode(outputs);
  std::set<vkShaderGraphCtx::ExternalBinding> bindings = ctx.GetBindingsFor(outputs);

  bool vsm = true;

  std::ostringstream ss;

  // generate the vertex shader
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "uniform mat4 vk_MatModel;" << std::endl
    << std::endl
    << "in vec4 vk_Position;" << std::endl
    << "in vec2 vk_TexCoord0;" << std::endl
    << std::endl
    // if we use hardware skinning this should be placed here
    << "out vec2 inGeomTexCoord;" << std::endl
    << std::endl
    << "void main()" << std::endl
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
    << "out vec2 inFragTexCoord;" << std::endl;
  if (vsm)
  {
    ss << "out float inFragDepth;" << std::endl;
  }
  ss << std::endl
    << "void main()" << std::endl
    << "{" << std::endl;

  for (unsigned i = 0; i < numLayers; ++i)
  {
    ss << ""
      << "  gl_Layer = " << i << ";" << std::endl;
    for (unsigned j = 0; j < 3; ++j)
    {
      ss << ""
        << "  gl_Position = vk_ShadowMapMatProjView[" << i << "] * gl_in[" << j << "].gl_Position;" << std::endl
        << "  inFragTexCoord = inGeomTexCoord[" << j << "];" << std::endl;
      if (vsm)
      {
        ss << "  inFragDepth = gl_Position.z / gl_Position.w;" << std::endl;
      }
      ss << "  EmitVertex();" << std::endl << std::endl;
    }
    ss << "  EndPrimitive();" << std::endl << std::endl << std::endl;
  }
  ss << "}" << std::endl;
  vkString geometryShaderSources = ss.str();



  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << (vsm
        ? "layout(location = 0) out vec2 vk_FragColor;"
        : "layout(location = 0) out vec4 vk_FragColor;"
        ) << std::endl
    << std::endl;
  for (const vkShaderGraphCtx::ExternalBinding &binding : bindings)
  {
    ss << "uniform " << binding.variableType << " " << binding.variableName << ";" << std::endl;
  }
  ss << std::endl
    << "in vec2 inFragTexCoord;" << std::endl;
  if (vsm)
  {
    ss << "in float inFragDepth;" << std::endl;
  }
  ss << std::endl
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
    ss << "  float dx = dFdx(inFragDepth);" << std::endl
      << "  float dy = dFdy(inFragDepth);" << std::endl
      // << "  vk_FragColor = vec2(inFragDepth, inFragDepth*inFragDepth + 0.25*(dx*dx + dy*dy));" << std::endl
      << "  vk_FragColor = vec2(abs(dx) + abs(dy), 1.0);" << std::endl
      ;
    ;
  }
  else
  {
    ss << "  vk_FragColor = vec4(1, 1, 1, 1);" << std::endl;
  }
  // if VSM this can be 
  ss << "}" << std::endl
    << std::endl;
  vkString fragmentShaderSources = ss.str();

  ss.clear();

  /*
  printf("Shadow[%d].Vertex:\n%s\n", numLayers, vertexShaderSources.c_str());
  printf("Shadow[%d].Geometry:\n%s\n", numLayers, geometryShaderSources.c_str());
  printf("Shadow[%d].Fragment:\n%s\n", numLayers, fragmentShaderSources.c_str());

  DebugCode("Shadow.Vertex", vertexShaderSources.c_str());
  DebugCode("Shadow.Geometry", geometryShaderSources.c_str());
  DebugCode("Shadow.Fragment", fragmentShaderSources.c_str());
  */


  vkShaderGL4 *vertexShader = new vkShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexShaderSources);
  if (!vertexShader->Compile())
  {
    printf("Error geometry shader:\n%s\n", vertexShader->GetCompileErrorLog().c_str());
    vertexShader->Release();
    return;
  }

  vkShaderGL4 *geometryShader = new vkShaderGL4();
  geometryShader->SetShaderType(eST_Geometry);
  geometryShader->SetSource(geometryShaderSources);
  if (!geometryShader->Compile())
  {
    printf("Error geometry shader:\n%s\n", geometryShader->GetCompileErrorLog().c_str());
    vertexShader->Release();
    geometryShader->Release();
    return;
  }

  vkShaderGL4 *fragmentShader = new vkShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentShaderSources);
  if (!fragmentShader->Compile())
  {
    printf("Error fragment shader:\n%s\n", fragmentShader->GetCompileErrorLog().c_str());
    vertexShader->Release();
    geometryShader->Release();
    fragmentShader->Release();
    return;
  }

  vkProgramGL4 *shadowShader = new vkProgramGL4();
  shadowShader->AttachShader(vertexShader);
  shadowShader->AttachShader(geometryShader);
  shadowShader->AttachShader(fragmentShader);
  if (!shadowShader->Link())
  {
    printf("Error link:\n%s\n", shadowShader->GetLinkErrorLog().c_str());
    vertexShader->Release();
    geometryShader->Release();
    fragmentShader->Release();
    shadowShader->Release();
    return;
  }


  graph->SetShader(renderPass, shadowShader);

}
