

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
{

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
