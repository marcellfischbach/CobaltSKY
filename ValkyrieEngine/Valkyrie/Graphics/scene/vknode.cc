
#include <Valkyrie/Graphics/scene/vknode.hh>
#include <Valkyrie/Graphics/scene/vkgroupnode.hh>



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



void vkNode::UpdateBoundingBox(vkBoundingBox &bbox)
{
  bbox.Clear();
}


void vkNode::UpdateStates()
{
  if (m_updateFlags & eUF_UpdateBoundingBox)
  {
    m_updateFlags &= ~eUF_UpdateBoundingBox;
    UpdateBoundingBox(m_boundingBox);
    //m_boundingBox.Debug(m_name.c_str());
  }
}

void vkNode::Scan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{
  //
  // TODO: handle the clipping of the bounding box here
  if (clipper)
  {
    vkClipper::ClipResult rc = clipper->Test(m_boundingBox);
    switch (rc)
    {
    case vkClipper::eCR_In:
      clipper = 0;
      break;
    case vkClipper::eCR_Out:
      return;
    }
  }

  PrivScan(clipper, renderer, callback);
}

void vkNode::PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback)
{

}