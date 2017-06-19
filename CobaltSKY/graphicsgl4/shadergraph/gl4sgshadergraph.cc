
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
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgnormalize.hh>
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
#include <valkyrie/core/csresourcemanager.hh>
#include <valkyrie/graphics/itexture.hh>

#include <valkyrie/graphics/shadergraph/cssgadd.hh>
#include <valkyrie/graphics/shadergraph/cssgconstfloat1.hh>
#include <valkyrie/graphics/shadergraph/cssgconstfloat2.hh>
#include <valkyrie/graphics/shadergraph/cssgconstfloat3.hh>
#include <valkyrie/graphics/shadergraph/cssgconstfloat4.hh>
#include <valkyrie/graphics/shadergraph/cssgcross.hh>
#include <valkyrie/graphics/shadergraph/cssgdefaulttexturecoordinate.hh>
#include <valkyrie/graphics/shadergraph/cssgdiv.hh>
#include <valkyrie/graphics/shadergraph/cssgdot.hh>
#include <valkyrie/graphics/shadergraph/cssgfloat2.hh>
#include <valkyrie/graphics/shadergraph/cssgfloat3.hh>
#include <valkyrie/graphics/shadergraph/cssgfloat4.hh>
#include <valkyrie/graphics/shadergraph/cssglerp.hh>
#include <valkyrie/graphics/shadergraph/cssgmul.hh>
#include <valkyrie/graphics/shadergraph/cssgnormalize.hh>
#include <valkyrie/graphics/shadergraph/cssgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/cssgsplitfloat2.hh>
#include <valkyrie/graphics/shadergraph/cssgsplitfloat3.hh>
#include <valkyrie/graphics/shadergraph/cssgsplitfloat4.hh>
#include <valkyrie/graphics/shadergraph/cssgsub.hh>
#include <valkyrie/graphics/shadergraph/cssgtexture2d.hh>
#include <valkyrie/graphics/shadergraph/cssgvarfloat1.hh>
#include <valkyrie/graphics/shadergraph/cssgvarfloat2.hh>
#include <valkyrie/graphics/shadergraph/cssgvarfloat3.hh>
#include <valkyrie/graphics/shadergraph/cssgvarfloat4.hh>

#include <iostream>
#include <sstream>

csShaderGraphGL4::csShaderGraphGL4(csGraphicsGL4 *graphics)
  : m_graphics(graphics)
{
}


csShaderGraphGL4::~csShaderGraphGL4()
{

}

csSGNodeGL4 *csShaderGraphGL4::CreateNode(const csClass *nodeClass)
{
  static bool initialized = false;
  if (!initialized)
  {
    m_classMapping.push_back(Map(csSGVarFloat::GetStaticClass(), csSGVarFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGVarFloat2::GetStaticClass(), csSGVarFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGVarFloat3::GetStaticClass(), csSGVarFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGVarFloat4::GetStaticClass(), csSGVarFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGConstFloat::GetStaticClass(), csSGConstFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGConstFloat2::GetStaticClass(), csSGConstFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGConstFloat3::GetStaticClass(), csSGConstFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGConstFloat4::GetStaticClass(), csSGConstFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGFloat2::GetStaticClass(), csSGFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGFloat3::GetStaticClass(), csSGFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGFloat4::GetStaticClass(), csSGFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGSplitFloat2::GetStaticClass(), csSGSplitFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGSplitFloat3::GetStaticClass(), csSGSplitFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGSplitFloat4::GetStaticClass(), csSGSplitFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGAdd::GetStaticClass(), csSGAddGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGSub::GetStaticClass(), csSGSubGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGMul::GetStaticClass(), csSGMulGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGDiv::GetStaticClass(), csSGDivGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGDot::GetStaticClass(), csSGDotGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGCross::GetStaticClass(), csSGCrossGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGLerp::GetStaticClass(), csSGLerpGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGNormalize::GetStaticClass(), csSGNormalizeGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGDefaultTextureCoordinate::GetStaticClass(), csSGDefaultTextureCoordinateGL4::GetStaticClass()));
    m_classMapping.push_back(Map(csSGTexture2D::GetStaticClass(), csSGTexture2DGL4::GetStaticClass()));
    initialized = true;
  }
  for (size_t i = 0, in = m_classMapping.size(); i < in; ++i)
  {
    Map &m = m_classMapping[i];
    if (m.nodeClass == nodeClass)
    {
      return m.nodeClassGL4->CreateInstance<csSGNodeGL4>();
    }
  }
  return 0;

}


csString csShaderGraphGL4::CreateCode(csSGNode *node, csSize output)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(output));
}

csString csShaderGraphGL4::CreateCode(csSGNode *node, const csString &outputName)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(outputName));
}


csString csShaderGraphGL4::CreateCode(csSGOutput *output)
{
  csShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  return ctx.CreateCode(output);
}



bool csShaderGraphGL4::GenerateShaderGraph(csSGShaderGraph *graph, iSGShaderGraphLogger *logger)
{
  if (!graph->Validate())
  {
    return false;
  }
  graph->ClearParameters();

  GenerateGBuffer(graph, logger);
  //GenerateShadow(graph, 1, eRP_ShadowGeneric);
  GenerateShadow(graph, 3, eRP_ShadowPSSM, logger);
  GenerateShadow(graph, 6, eRP_ShadowCube, logger);

  for (size_t i = 0, in = graph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    csSGNode *node = graph->GetNode(i);
    if (!node)
    {
      continue;
    }

    csSGResourceNode *resNode = csQueryClass<csSGResourceNode>(node);
    if (!resNode)
    {
      continue;
    }

    const csString &resName = resNode->GetResourceName();
    csShaderParameterType resType = resNode->GetResourceType();

    csSize idx = graph->RegisterParam(resName, resType);

    float *floats = resNode->GetDefaultFloats();
    int *ints = resNode->GetDefaultInts();
    switch (resType)
    {
    case eSPT_Float:
      graph->SetDefault(idx, floats[0]);
      break;
    case eSPT_Vector2:
      graph->SetDefault(idx, csVector2f(floats[0], floats[1]));
      break;
    case eSPT_Vector3:
      graph->SetDefault(idx, csVector3f(floats[0], floats[1], floats[2]));
      break;
    case eSPT_Vector4:
      graph->SetDefault(idx, csVector4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Color4:
      graph->SetDefault(idx, csColor4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Texture:
    {
      iTexture *texture = csResourceManager::Get()->GetOrLoad<iTexture>(resNode->GetDefaultTextureResource());
      graph->SetDefault(idx, texture);
    }
    break;
    case eSPT_Int:
    case eSPT_IVector2:
    case eSPT_IVector3:
    case eSPT_IVector4:
      break;
    }
  }

  return true;
}



void csShaderGraphGL4::DebugCode(const csString &title, const csString &code)
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
