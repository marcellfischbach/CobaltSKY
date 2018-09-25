#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/graphics/scene/csnode.hh>
#include <vector>
#include <cobalt/graphics/scene/csgroupnode.refl.hh>

CS_CLASS()
class CSE_API csGroupNode : public CS_SUPER(csNode)
{
  friend class csNode;
  CS_CLASS_GEN;

public:
  csGroupNode();
  virtual ~csGroupNode();

  virtual void UpdateStates();

protected:
  virtual void PrivScan(const csClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  virtual void UpdateBoundingBox(csBoundingBox &bbox);
private:

  bool AddChild(csNode *child);
  bool RemoveChild(csNode *child);

  std::vector<csNode *> m_children;

};
