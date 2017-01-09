#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/graphics/scene/vknode.hh>
#include <vector>
#include <valkyrie/graphics/scene/vkgroupnode.refl.hh>

VK_CLASS()
class VKE_API vkGroupNode : public VK_SUPER(vkNode)
{
  friend class vkNode;
  VK_CLASS_GEN;

public:
  vkGroupNode();
  virtual ~vkGroupNode();

  virtual void UpdateStates();

protected:
  virtual void PrivScan(const vkClipper *clipper, iGraphics *renderer, iScanCallback *callback);

  virtual void UpdateBoundingBox(vkBoundingBox &bbox);
private:

  bool AddChild(vkNode *child);
  bool RemoveChild(vkNode *child);

  std::vector<vkNode *> m_children;

};
