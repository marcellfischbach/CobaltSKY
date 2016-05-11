
#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <iostream>
#include <sstream>

vkShaderGraphGL4::vkShaderGraphGL4(vkGraphicsGL4 *graphics)
  : m_graphics(graphics)
{
}


vkShaderGraphGL4::~vkShaderGraphGL4()
{

}

vkSGNodeGL4 *vkShaderGraphGL4::CreateNode(const vkClass *nodeClass)
{
  static bool initialized = false;
  if (!initialized)
  {
    m_classMapping.push_back(Map(vkSGVarFloat::GetStaticClass(), vkSGVarFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGVarFloat2::GetStaticClass(), vkSGVarFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGVarFloat3::GetStaticClass(), vkSGVarFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGVarFloat4::GetStaticClass(), vkSGVarFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGConstFloat::GetStaticClass(), vkSGConstFloatGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGConstFloat2::GetStaticClass(), vkSGConstFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGConstFloat3::GetStaticClass(), vkSGConstFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGConstFloat4::GetStaticClass(), vkSGConstFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGFloat2::GetStaticClass(), vkSGFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGFloat3::GetStaticClass(), vkSGFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGFloat4::GetStaticClass(), vkSGFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGSplitFloat2::GetStaticClass(), vkSGSplitFloat2GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGSplitFloat3::GetStaticClass(), vkSGSplitFloat3GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGSplitFloat4::GetStaticClass(), vkSGSplitFloat4GL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGAdd::GetStaticClass(), vkSGAddGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGSub::GetStaticClass(), vkSGSubGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGMul::GetStaticClass(), vkSGMulGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGDiv::GetStaticClass(), vkSGDivGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGDot::GetStaticClass(), vkSGDotGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGCross::GetStaticClass(), vkSGCrossGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGLerp::GetStaticClass(), vkSGLerpGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGDefaultTextureCoordinate::GetStaticClass(), vkSGDefaultTextureCoordinateGL4::GetStaticClass()));
    m_classMapping.push_back(Map(vkSGTexture2D::GetStaticClass(), vkSGTexture2DGL4::GetStaticClass()));
    initialized = true;
  }
  for (size_t i = 0, in = m_classMapping.size(); i < in; ++i)
  {
    Map &m = m_classMapping[i];
    if (m.nodeClass == nodeClass)
    {
      return m.nodeClassGL4->CreateInstance<vkSGNodeGL4>();
    }
  }
  return 0;

}


vkString vkShaderGraphGL4::CreateCode(vkSGNode *node, vkSize output)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(output));
}

vkString vkShaderGraphGL4::CreateCode(vkSGNode *node, const vkString &outputName)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(outputName));
}


vkString vkShaderGraphGL4::CreateCode(vkSGOutput *output)
{
  vkShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  return ctx.CreateCode(output);
}



bool vkShaderGraphGL4::GenerateShaderGraph(vkSGShaderGraph *graph)
{
  if (!graph->Validate())
  {
    return false;
  }
  graph->ClearParameters();

  GenerateGBuffer(graph);
  //GenerateShadow(graph, 1, eRP_ShadowGeneric);
  //GenerateShadow(graph, 3, eRP_ShadowPSSM);
  //GenerateShadow(graph, 6, eRP_ShadowCube);

  for (size_t i = 0, in = graph->GetNumberOfTotalNodes(); i < in; ++i)
  {
    vkSGNode *node = graph->GetNode(i);
    if (!node)
    {
      continue;
    }

    vkSGResourceNode *resNode = vkQueryClass<vkSGResourceNode>(node);
    if (!resNode)
    {
      continue;
    }

    const vkString &resName = resNode->GetResourceName();
    vkShaderParameterType resType = resNode->GetResourceType();

    vkSize idx = graph->RegisterParam(resName, resType);

    float *floats = resNode->GetDefaultFloats();
    int *ints = resNode->GetDefaultInts();
    switch (resType)
    {
    case eSPT_Float:
      graph->SetDefault(idx, floats[0]);
      break;
    case eSPT_Vector2:
      graph->SetDefault(idx, vkVector2f(floats[0], floats[1]));
      break;
    case eSPT_Vector3:
      graph->SetDefault(idx, vkVector3f(floats[0], floats[1], floats[2]));
      break;
    case eSPT_Vector4:
      graph->SetDefault(idx, vkVector4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Color4:
      graph->SetDefault(idx, vkColor4f(floats[0], floats[1], floats[2], floats[3]));
      break;
    case eSPT_Texture:
      {
        ITexture *texture = vkResourceManager::Get()->GetOrLoad<ITexture>(resNode->GetDefaultTextureResource());
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



void vkShaderGraphGL4::DebugCode(const vkString &title, const vkString &code)
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