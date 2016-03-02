
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
    m_classMapping[vkSGVarFloat::GetStaticClass()] = vkSGVarFloatGL4::GetStaticClass();
    m_classMapping[vkSGVarFloat2::GetStaticClass()] = vkSGVarFloat2GL4::GetStaticClass();
    m_classMapping[vkSGVarFloat3::GetStaticClass()] = vkSGVarFloat3GL4::GetStaticClass();
    m_classMapping[vkSGVarFloat4::GetStaticClass()] = vkSGVarFloat4GL4::GetStaticClass();
    m_classMapping[vkSGConstFloat::GetStaticClass()] = vkSGConstFloatGL4::GetStaticClass(); 
    m_classMapping[vkSGConstFloat2::GetStaticClass()] = vkSGConstFloat2GL4::GetStaticClass();
    m_classMapping[vkSGConstFloat3::GetStaticClass()] = vkSGConstFloat3GL4::GetStaticClass();
    m_classMapping[vkSGConstFloat4::GetStaticClass()] = vkSGConstFloat4GL4::GetStaticClass();
    m_classMapping[vkSGFloat2::GetStaticClass()] = vkSGFloat2GL4::GetStaticClass();
    m_classMapping[vkSGAdd::GetStaticClass()] = vkSGAddGL4::GetStaticClass();
    m_classMapping[vkSGSub::GetStaticClass()] = vkSGSubGL4::GetStaticClass();
    m_classMapping[vkSGDefaultTextureCoordinate::GetStaticClass()] = vkSGDefaultTextureCoordinateGL4::GetStaticClass();
    m_classMapping[vkSGTexture2D::GetStaticClass()] = vkSGTexture2DGL4::GetStaticClass();

    initialized = true;
  }
  std::map<const vkClass*, const vkClass*>::iterator it = m_classMapping.find(nodeClass);
  if (it == m_classMapping.end())
  {
    return 0;
  }

  return it->second->CreateInstance<vkSGNodeGL4>();
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

