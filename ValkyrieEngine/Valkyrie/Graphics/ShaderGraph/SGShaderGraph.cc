

#include <Valkyrie/Graphics/ShaderGraph/SGShaderGraph.hh>
#include <Valkyrie/Graphics/ShaderGraph/SGNode.hh>


vkSGShaderGraph::vkSGShaderGraph()
  : vkMaterial()
  , m_diffuse(0)
  , m_roughness(0)
  , m_alpha(0)
  , m_discardAlpha(false)
  , m_discardAlphaThreshold(0.0f)
  , m_discardAlphaCompareMode(eCM_Less)
  , m_normal(0)
{

}


void vkSGShaderGraph::SetInput(vkSGShaderGraph::InputType type, vkSGOutput *output)
{
  switch (type)
  {
  case eIT_Diffuse:
    SetDiffuse(output);
    break;
  case eIT_Roughness:
    SetRoughness(output);
    break;
  case eIT_Alpha:
    SetAlpha(output);
    break;
  case eIT_Normal:
    SetNormal(output);
    break;
  }
}

vkSGOutput *vkSGShaderGraph::GetInput(vkSGShaderGraph::InputType type)
{
  switch (type)
  {
  case eIT_Diffuse:
    return GetDiffuse();
  case eIT_Roughness:
    return GetRoughness();
  case eIT_Alpha:
    return GetAlpha();
  case eIT_Normal:
    return GetNormal();
  }
}

bool vkSGShaderGraph::Validate()
{
  bool success = true;

  success &= ValidateNode(m_diffuse);
  success &= ValidateNode(m_alpha);

  return success;
}


vkSGNode *vkSGShaderGraph::GetNode(vkSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return output->GetNode();
}

bool vkSGShaderGraph::ValidateNode(vkSGOutput *output)
{
  vkSGNode *node = GetNode(output);
  if (!node)
  {
    return true;
  }

  return node->Validate();
}


void vkSGShaderGraph::AddNode(vkSGNode *node)
{
  if (!node)
  {
    return;
  }
  node->AddRef();
  m_allNodes.push_back(node);
}

size_t vkSGShaderGraph::GetNumberOfTotalNodes() const
{
  return m_allNodes.size();
}

vkSGNode* vkSGShaderGraph::GetNode(size_t idx)
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}

const vkSGNode* vkSGShaderGraph::GetNode(size_t idx) const
{
  if (idx >= m_allNodes.size())
  {
    return 0;
  }
  return m_allNodes[idx];
}
