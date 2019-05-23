
#include <cobalt/graphics/scene/csgroupnode.hh>


cs::GroupNode::GroupNode()
  : cs::Node()
{

}

cs::GroupNode::~GroupNode()
{

}

bool cs::GroupNode::AddChild(cs::Node *node)
{
  node->AddRef();
  m_children.push_back(node);
  return true;
}


bool cs::GroupNode::RemoveChild(cs::Node *node)
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

void cs::GroupNode::PrivScan(const cs::Clipper *clipper, cs::iGraphics *renderer, cs::iScanCallback *callback)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Scan(clipper, renderer, callback);
  }
}

void cs::GroupNode::UpdateStates()
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->UpdateStates();
  }

  cs::Node::UpdateStates();

}

void cs::GroupNode::UpdateBoundingBox(cs::BoundingBox &bbox)
{
  cs::Node::UpdateBoundingBox(bbox);
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    bbox.Add(m_children[i]->GetBoundingBox());
  }
  bbox.Finish();

}