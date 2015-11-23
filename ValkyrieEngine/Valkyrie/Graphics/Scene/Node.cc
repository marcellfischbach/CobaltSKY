
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Graphics/Scene/GroupNode.hh>



vkNode::vkNode()
  : vkObject()
  , m_parent(0)
{
  m_updateFlags = 0x00;
}


vkNode::~vkNode()
{

}

void vkNode::SetParent(vkGroupNode *parent)
{
  VK_SET(m_parent, parent);
}

bool vkNode::AttachTo(vkGroupNode *parent)
{
  if (m_parent || !parent)
  {
    return false;
  }

  SetParent(parent);
  parent->AddChild(this);
  FlagUpdateBoundingBox();
  return true;
}

void vkNode::DetachFromParent()
{
  if (!m_parent)
  {
    return;
  }
  m_parent->RemoveChild(this);
  SetParent(0);
  FlagUpdateBoundingBox();
}


void vkNode::FlagUpdateBoundingBox()
{
  if (m_updateFlags & eUF_UpdateBoundingBox)
  {
    return;
  }

  m_updateFlags |= eUF_UpdateBoundingBox;
  if (m_parent)
  {
    m_parent->FlagUpdateBoundingBox();
  }
}



void vkNode::UpdateBoundingBox()
{
  m_boundingBox.Clear();
}


void vkNode::UpdateStates()
{
  if (m_updateFlags & eUF_UpdateBoundingBox)
  {
    m_updateFlags &= ~eUF_UpdateBoundingBox;
    UpdateBoundingBox();
  }
}

void vkNode::Scan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{
  //
  // TODO: handle the clipping of the bounding box here

  PrivScan(clipper, renderer, callback);
}

void vkNode::PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{

}