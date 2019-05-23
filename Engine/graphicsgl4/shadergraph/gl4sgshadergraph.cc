
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgadd.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat1.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat3.hh>
#include <graphicsgl4/shadergraph/gl4sgconstfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgcross.hh>
#include <graphicsgl4/shadergraph/gl4sgdefaulttexturecoordinate.hh>
#include <graphicsgl4/shadergraph/gl4sgdiv.hh>
#include <graphicsgl4/shadergraph/gl4sgdot.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat3.hh>
#include <graphicsgl4/shadergraph/gl4sgfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sglerp.hh>
#include <graphicsgl4/shadergraph/gl4sgmul.hh>
#include <graphicsgl4/shadergraph/gl4sgneg.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnormalize.hh>
#include <graphicsgl4/shadergraph/gl4sgoneby.hh>
#include <graphicsgl4/shadergraph/gl4sgoneminus.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat3.hh>
#include <graphicsgl4/shadergraph/gl4sgsplitfloat4.hh>
#include <graphicsgl4/shadergraph/gl4sgsub.hh>
#include <graphicsgl4/shadergraph/gl4sgtexture2d.hh>
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat1.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat2.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat3.hh>
#include <graphicsgl4/shadergraph/gl4sgvarfloat4.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <cobalt/core/csresourcemanager.hh>
#include <cobalt/graphics/itexture.hh>

#include <cobalt/graphics/shadergraph/cssgadd.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat1.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat4.hh>
#include <cobalt/graphics/shadergraph/cssgcross.hh>
#include <cobalt/graphics/shadergraph/cssgdefaulttexturecoordinate.hh>
#include <cobalt/graphics/shadergraph/cssgdiv.hh>
#include <cobalt/graphics/shadergraph/cssgdot.hh>
#include <cobalt/graphics/shadergraph/cssgfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgfloat4.hh>
#include <cobalt/graphics/shadergraph/cssglerp.hh>
#include <cobalt/graphics/shadergraph/cssgmul.hh>
#include <cobalt/graphics/shadergraph/cssgneg.hh>
#include <cobalt/graphics/shadergraph/cssgnormalize.hh>
#include <cobalt/graphics/shadergraph/cssgoneby.hh>
#include <cobalt/graphics/shadergraph/cssgoneminus.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.hh>
#include <cobalt/graphics/shadergraph/cssgsub.hh>
#include <cobalt/graphics/shadergraph/cssgtexture2d.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat1.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat2.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat3.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat4.hh>

#include <iostream>
#include <sstream>

cs::ShaderGraphGL4::ShaderGraphGL4(cs::GraphicsGL4 *graphics)
  : m_graphics(graphics)
{
}


cs::ShaderGraphGL4::~ShaderGraphGL4()
{

}

cs::SGNodeGL4 *cs::ShaderGraphGL4::CreateNode(const cs::Class *nodeClass)
{
  static bool initialized = false;
  if (!initialized)
  {
    m_classMapping.push_back(Map(cs::SGVarFloat::GetStaticClass(), cs::SGVarFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGVarFloat2::GetStaticClass(), cs::SGVarFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGVarFloat3::GetStaticClass(), cs::SGVarFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGVarFloat4::GetStaticClass(), cs::SGVarFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGConstFloat::GetStaticClass(), cs::SGConstFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGConstFloat2::GetStaticClass(), cs::SGConstFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGConstFloat3::GetStaticClass(), cs::SGConstFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGConstFloat4::GetStaticClass(), cs::SGConstFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGFloat2::GetStaticClass(), cs::SGFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGFloat3::GetStaticClass(), cs::SGFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGFloat4::GetStaticClass(), cs::SGFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGSplitFloat2::GetStaticClass(), cs::SGSplitFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGSplitFloat3::GetStaticClass(), cs::SGSplitFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGSplitFloat4::GetStaticClass(), cs::SGSplitFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGAdd::GetStaticClass(), cs::SGAddGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGSub::GetStaticClass(), cs::SGSubGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGMul::GetStaticClass(), cs::SGMulGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGDiv::GetStaticClass(), cs::SGDivGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGNeg::GetStaticClass(), cs::SGNegGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGOneBy::GetStaticClass(), cs::SGOneByGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGOneMinus::GetStaticClass(), cs::SGOneMinusGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGDot::GetStaticClass(), cs::SGDotGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGCross::GetStaticClass(), cs::SGCrossGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGLerp::GetStaticClass(), cs::SGLerpGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGNormalize::GetStaticClass(), cs::SGNormalizeGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGDefaultTextureCoordinate::GetStaticClass(), cs::SGDefaultTextureCoordinateGL4::GetStaticClass()));
    m_classMapping.push_back(Map(cs::SGTexture2D::GetStaticClass(), cs::SGTexture2DGL4::GetStaticClass()));
    initialized = true;
  }
  for (size_t i = 0, in = m_classMapping.size(); i < in; ++i)
  {
    Map &m = m_classMapping[i];
    if (m.nodeClass == nodeClass)
    {
      return m.nodeClassGL4->CreateInstance<cs::SGNodeGL4>();
    }
  }
  return 0;

}


std::string cs::ShaderGraphGL4::CreateCode(cs::SGNode *node, csSize output)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(output));
}

std::string cs::ShaderGraphGL4::CreateCode(cs::SGNode *node, const std::string &outputName)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(outputName));
}


std::string cs::ShaderGraphGL4::CreateCode(cs::SGOutput *output)
{
  cs::ShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  return ctx.CreateCode(output);
}



bool cs::ShaderGraphGL4::GenerateShaderGraph(cs::SGShaderGraph *graph, cs::iSGShaderGraphLogger *logger)
{
  if (!graph->Validate())
  {
    return false;
  }
  graph->ClearParameters();

  GenerateGBuffer(graph, logger);
  //GenerateShadow(graph, 1, cs::eRP_ShadowGeneric);
  GenerateShadow(graph, 3, cs::eRP_ShadowPSSM, logger);
  GenerateShadow(graph, 6, cs::eRP_ShadowCube, logger);

  for (size_t i = 0, in = graph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    cs::SGNode *node = graph->GetNode(i);
    if (!node)
    {
      continue;
    }

    cs::SGResourceNode *resNode = cs::QueryClass<cs::SGResourceNode>(node);
    if (!resNode)
    {
      continue;
    }

    const std::string &resId = resNode->GetResourceId();
    const std::string &resName = resNode->GetResourceName();
    cs::eShaderParameterType resType = resNode->GetResourceType();

    csSize idx = graph->RegisterParam(resId, resName, resType);

    float *floats = resNode->GetDefaultFloats();
    int *ints = resNode->GetDefaultInts();
    switch (resType)
    {
    case cs::eSPT_Float:
      graph->SetDefault(idx, floats[0]);
      break;
    case cs::eSPT_Vector2:
      graph->SetDefault(idx, cs::Vector2f(floats[0], floats[1]));
      break;
    case cs::eSPT_Vector3:
      graph->SetDefault(idx, cs::Vector3f(floats[0], floats[1], floats[2]));
      break;
    case cs::eSPT_Vector4:
      graph->SetDefault(idx, cs::Vector4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case cs::eSPT_Color4:
      graph->SetDefault(idx, cs::Color4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case cs::eSPT_Texture:
    {
      cs::TextureWrapper *texture = cs::ResourceManager::Get()->GetOrLoad<cs::TextureWrapper>(resNode->GetDefaultTextureResource());
      graph->SetDefault(idx, texture);
    }
    break;
    case cs::eSPT_Int:
    case cs::eSPT_IVector2:
    case cs::eSPT_IVector3:
    case cs::eSPT_IVector4:
      break;
    }
  }

  return true;
}



void cs::ShaderGraphGL4::DebugCode(const std::string &title, const std::string &code)
{
  printf("%s\n", title.c_str());

  unsigned line = 1;
  printf("(%02d) ", line++);
  for (size_t i = 0, in = code.length(); i < in; ++i)
  {
    if (code[i] == '\n')
    {
      printf("\n(%02d) ", line++);
    }
    else if (code[i] == '\r')
    {
      continue;
    }
    else
    {
      printf("%c", code[i]);
    }
  }
  printf("\n");
}
