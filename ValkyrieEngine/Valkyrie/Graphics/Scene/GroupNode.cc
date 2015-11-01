
#include <Valkyrie/Graphics/Scene/GroupNode.hh>


vkGroupNode::vkGroupNode()
  : vkNode()
{

}

vkGroupNode::~vkGroupNode()
{

}

bool vkGroupNode::AddChild(vkNode *node)
{
  node->AddRef();
  m_children.push_back(node);
  return true;
}


bool vkGroupNode::RemoveChild(vkNode *node)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    if (m_children[i] == node)
    {
      // remove the parent
      node->Release();
      m_children.erase(m_children.begin() + i);
      return true;
    }
  }
  return false;
}

void vkGroupNode::PrivScan(const vkClipper *clipper, IRenderer *renderer, IScanCallback *callback)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Scan(clipper, renderer, callback);
  }
}