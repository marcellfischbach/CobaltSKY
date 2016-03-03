
#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.hh>
#include <GraphicsGL4/GraphicsGL4.hh>
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
  GenerateGBuffer(graph);

  return true;
}

