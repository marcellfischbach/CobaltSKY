
#include <cobalt/graphics/scene/csgroupnode.hh>


csGroupNode::csGroupNode()
  : csNode()
{

}

csGroupNode::~csGroupNode()
{

}

bool csGroupNode::AddChild(csNode *node)
{
  node->AddRef();
  m_children.push_back(node);
  return true;
}


bool csGroupNode::RemoveChild(csNode *node)
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

void csGroupNode::PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->Scan(clipper, renderer, callback);
  }
}

void csGroupNode::UpdateStates()
{
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    m_children[i]->UpdateStates();
  }

  csNode::UpdateStates();

}

void csGroupNode::UpdateBoundingBox(csBoundingBox &bbox)
{
  csNode::UpdateBoundingBox(bbox);
  for (size_t i = 0, in = m_children.size(); i < in; ++i)
  {
    bbox.Add(m_children[i]->GetBoundingBox());
  }
  bbox.Finish();

}