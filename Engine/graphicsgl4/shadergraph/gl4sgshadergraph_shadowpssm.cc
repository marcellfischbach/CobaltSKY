#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4program.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>
#include <iostream>
#include <sstream>
#include <set>

static const char *compareMode[] = {
  "<=",
  ">=",
  "<",
  ">",
  "==",
  "!="
};

void cs::ShaderGraphGL4::GenerateShadow(cs::SGShaderGraph *graph, unsigned numLayers, cs::eRenderPass renderPass, cs::iSGShaderGraphLogger *logger)
{
  std::string passName;
  switch (renderPass)
  {
  case cs::eRP_ShadowPSSM:
    passName = "ShadowPSSM";
    break;
  case cs::eRP_ShadowCube:
    passName = "ShadowCube";
    break;
  }
  graph->SetShader(renderPass, 0);

  std::set<cs::SGOutput*> outputs;
  std::set<cs::SGOutput*> preAlphaOutputs;
  std::set<cs::SGOutput*> postAlphaOutputs;

  cs::SGOutput *alphaOutput = graph->GetAlpha();
  if (alphaOutput)
  {
    if (alphaOutput->GetDataType() != cs::eSGDT_Float)
    {
      alphaOutput = 0;
    }
    else
    {
      outputs.insert(alphaOutput);
      preAlphaOutputs.insert(alphaOutput);
    }
  }

  cs::ShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  ctx.EvaluateInlines(outputs);

  ctx.GenerateCode(preAlphaOutputs);
  std::string preAlphaCode = ctx.GetCode();

  ctx.GenerateCode(outputs);
  std::set<cs::ShaderGraphCtx::ExternalBinding> bindings = ctx.GetBindingsFor(outputs);

  bool vsm = true;

  std::ostringstream ss;

  // generate the vertex shader
  ss << ""
    << "#version 330" << std::endl
    << std::endl 
    << "uniform mat4 cs_MatModel;" << std::endl
    << std::endl
    << "in vec4 cs_Position;" << std::endl
    << "in vec2 cs_TexCoord0;" << std::endl;
  if (graph->IsSkinnedMaterial())
  {
    ss // add all the bindings that are needed to correctly perform multi bone skinning
      << "uniform mat4 cs_MatsSkeleton[" << graph->GetMaxBones() << "];" << std::endl
      << "uniform int cs_SkeletonMapping[" << graph->GetMaxBones() << "];" << std::endl
      << "in ivec4 cs_BoneIndex;" << std::endl
      << "in vec4 cs_BoneWeight;" << std::endl;
  }

  ss
    << std::endl
    // if we use hardware skinning this should be placed here
    << "out vec2 inGeomTexCoord;" << std::endl
    << std::endl
    << "void main()" << std::endl
    << "{" << std::endl;
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
      << "  vec4 position = boneMat40 * cs_Position * cs_BoneWeight.x + " << std::endl
      << "                  boneMat41 * cs_Position * cs_BoneWeight.y + " << std::endl
      << "                  boneMat42 * cs_Position * cs_BoneWeight.z + " << std::endl
      << "                  boneMat43 * cs_Position * cs_BoneWeight.w;" << std::endl
      << "  gl_Position = cs_MatModel * position;" << std::endl;
  }
  else
  {
    ss
      << "  gl_Position = cs_MatModel * cs_Position;" << std::endl;
  }
  ss
    << "  inGeomTexCoord = cs_TexCoord0;" << std::endl
    << "}" << std::endl
    << std::endl;
  std::string vertexShaderSources = ss.str();
  if (logger)
  {
    logger->LogSourceCode(passName, "VertexShader", vertexShaderSources);
  }

  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << "layout(triangles) in;" << std::endl
    << "layout(triangle_strip, max_vertices = " << (numLayers * 3) << ") out;" << std::endl
    << "uniform mat4 cs_ShadowMapMatProjView[" << numLayers << "];" << std::endl
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
        << "  gl_Position = cs_ShadowMapMatProjView[" << i << "] * gl_in[" << j << "].gl_Position;" << std::endl
        << "  inFragTexCoord = inGeomTexCoord[" << j << "];" << std::endl;
      if (vsm)
      {
        ss << "  inFragDepth = gl_Position.z / gl_Position.w;" << std::endl
           << "  inFragDepth = inFragDepth * 0.5 + 0.5;" << std::endl
          ;
      }
      ss << "  EmitVertex();" << std::endl << std::endl;
    }
    ss << "  EndPrimitive();" << std::endl << std::endl << std::endl;
  }
  ss << "}" << std::endl;
  std::string geometryShaderSources = ss.str();
  if (logger)
  {
    logger->LogSourceCode(passName, "GeometryShader", geometryShaderSources);
  }



  ss = std::ostringstream();
  ss << ""
    << "#version 330" << std::endl
    << std::endl
    << (vsm
        ? "layout(location = 0) out vec2 cs_FragColor;"
        : "layout(location = 0) out vec4 cs_FragColor;"
        ) << std::endl
    << std::endl;
  for (const cs::ShaderGraphCtx::ExternalBinding &binding : bindings)
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
    ss
      << "  float dx = dFdx(inFragDepth);" << std::endl
      << "  float dy = dFdy(inFragDepth);" << std::endl
      //<< "  cs_FragColor = vec2(inFragDepth, inFragDepth*inFragDepth + 0.25*(dx*dx + dy*dy));" << std::endl
      << "  cs_FragColor = vec2(abs(dx) + abs(dy), 0.0);" << std::endl
      ;
    ;
  }
  else
  {
    ss << "  cs_FragColor = vec4(1, 1, 1, 1);" << std::endl;
  }
  // if VSM this can be 
  ss << "}" << std::endl
    << std::endl;
  std::string fragmentShaderSources = ss.str();
  if (logger)
  {
    logger->LogSourceCode(passName, "FragmentShader", fragmentShaderSources);
  }

  ss.clear();

  //DebugCode("PSSM.Vertex", vertexShaderSources.c_str());
  //DebugCode("PSSM.Geometry", geometryShaderSources.c_str());
  //DebugCode("PSSM.Fragment", fragmentShaderSources.c_str());


  cs::ShaderGL4 *vertexShader = new cs::ShaderGL4();
  vertexShader->SetShaderType(cs::eST_Vertex);
  vertexShader->SetSource(vertexShaderSources);
  if (!vertexShader->Compile())
  {
    if (logger)
    {
      logger->LogError(passName+".Vertex", vertexShader->GetCompileErrorLog());
    }

    vertexShader->Release();
    return;
  }

  cs::ShaderGL4 *geometryShader = new cs::ShaderGL4();
  geometryShader->SetShaderType(cs::eST_Geometry);
  geometryShader->SetSource(geometryShaderSources);
  if (!geometryShader->Compile())
  {
    if (logger)
    {
      logger->LogError(passName + ".Geometry", geometryShader->GetCompileErrorLog());
    }
    vertexShader->Release();
    geometryShader->Release();
    return;
  }

  cs::ShaderGL4 *fragmentShader = new cs::ShaderGL4();
  fragmentShader->SetShaderType(cs::eST_Fragment);
  fragmentShader->SetSource(fragmentShaderSources);
  if (!fragmentShader->Compile())
  {
    if (logger)
    {
      logger->LogError(passName + ".Fragment", fragmentShader->GetCompileErrorLog());
    }

    vertexShader->Release();
    geometryShader->Release();
    fragmentShader->Release();
    return;
  }

  cs::ProgramGL4 *shadowShader = new cs::ProgramGL4();
  shadowShader->AttachShader(new cs::ShaderGL4Wrapper(vertexShader));
  shadowShader->AttachShader(new cs::ShaderGL4Wrapper(geometryShader));
  shadowShader->AttachShader(new cs::ShaderGL4Wrapper(fragmentShader));
  if (!shadowShader->Link())
  {
    if (logger)
    {
      logger->LogError(passName + ".Link", shadowShader->GetLinkErrorLog());
    }

    vertexShader->Release();
    geometryShader->Release();
    fragmentShader->Release();
    shadowShader->Release();
    return;
  }


  graph->SetShader(renderPass, new cs::ProgramGL4Wrapper(shadowShader));

}
