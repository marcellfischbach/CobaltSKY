
#include <cobalt/graphics/scene/csnode.hh>
#include <cobalt/graphics/scene/csgroupnode.hh>



csNode::csNode()
  : csObject()
  , m_parent(0)
{
  m_updateFlags = 0x00;
}


csNode::~csNode()
{

}

void csNode::SetParent(csGroupNode *parent)
{
  CS_SET(m_parent, parent);
}

bool csNode::AttachTo(csGroupNode *parent)
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

void csNode::DetachFromParent()
{
  if (!m_parent)
  {
    return;
  }
  m_parent->RemoveChild(this);
  SetParent(0);
  FlagUpdateBoundingBox();
}


void csNode::FlagUpdateBoundingBox()
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



void csNode::UpdateBoundingBox(csBoundingBox &bbox)
{
  bbox.Clear();
}


void csNode::UpdateStates()
{
  if (m_updateFlags & eUF_UpdateBoundingBox)
  {
    m_updateFlags &= ~eUF_UpdateBoundingBox;
    UpdateBoundingBox(m_boundingBox);
    //m_boundingBox.Debug(m_name.c_str());
  }
}

void csNode::Scan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{
  //
  // TODO: handle the clipping of the bounding box here
  if (clipper)
  {
    csClipper::ClipResult rc = clipper->Test(m_boundingBox);
    switch (rc)
    {
    case csClipper::eCR_In:
      clipper = 0;
      break;
    case csClipper::eCR_Out:
      return;
    }
  }

  PrivScan(clipper, renderer, callback);
}

void csNode::PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback)
{

}