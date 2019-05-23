
#include <cobalt/graphics/scene/csnode.hh>
#include <cobalt/graphics/scene/csgroupnode.hh>



cs::Node::Node()
  : cs::Object()
  , m_parent(0)
{
  m_updateFlags = 0x00;
}


cs::Node::~Node()
{

}

void cs::Node::SetParent(cs::GroupNode *parent)
{
  CS_SET(m_parent, parent);
}

bool cs::Node::AttachTo(cs::GroupNode *parent)
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

void cs::Node::DetachFromParent()
{
  if (!m_parent)
  {
    return;
  }
  m_parent->RemoveChild(this);
  SetParent(0);
  FlagUpdateBoundingBox();
}


void cs::Node::FlagUpdateBoundingBox()
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



void cs::Node::UpdateBoundingBox(cs::BoundingBox &bbox)
{
  bbox.Clear();
}


void cs::Node::UpdateStates()
{
  if (m_updateFlags & eUF_UpdateBoundingBox)
  {
    m_updateFlags &= ~eUF_UpdateBoundingBox;
    UpdateBoundingBox(m_boundingBox);
    //m_boundingBox.Debug(m_name.c_str());
  }
}

void cs::Node::Scan(const cs::Clipper *clipper, cs::iGraphics *renderer, cs::iScanCallback *callback)
{
  //
  // TODO: handle the clipping of the bounding box here
  if (clipper)
  {
    cs::Clipper::ClipResult rc = clipper->Test(m_boundingBox);
    switch (rc)
    {
    case cs::Clipper::eCR_In:
      clipper = 0;
      break;
    case cs::Clipper::eCR_Out:
      return;
    }
  }

  PrivScan(clipper, renderer, callback);
}

void cs::Node::PrivScan(const cs::Clipper *clipper, cs::iGraphics *renderer, cs::iScanCallback *callback)
{

}