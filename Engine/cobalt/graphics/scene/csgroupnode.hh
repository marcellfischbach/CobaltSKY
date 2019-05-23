#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csnode.hh>
#include <vector>
#include <cobalt/graphics/scene/csgroupnode.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API GroupNode : public CS_SUPER(cs::Node)
{
  friend class cs::Node;
  CS_CLASS_GEN;

public:
  GroupNode();
  virtual ~GroupNode();

  virtual void UpdateStates();

protected:
  virtual void PrivScan(const cs::Clipper * clipper, cs::iGraphics * renderer, cs::iScanCallback * callback);

  virtual void UpdateBoundingBox(cs::BoundingBox & bbox);
private:

  bool AddChild(cs::Node * child);
  bool RemoveChild(cs::Node * child);

  std::vector<cs::Node*> m_children;

};

}
