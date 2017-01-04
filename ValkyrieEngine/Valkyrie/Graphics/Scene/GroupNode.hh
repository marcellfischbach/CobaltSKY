#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <vector>
#include <Valkyrie/Graphics/Scene/GroupNode.refl.hh>

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
  virtual void PrivScan(const vkClipper *clipper, IGraphics *renderer, IScanCallback *callback);

  virtual void UpdateBoundingBox(vkBoundingBox &bbox);
private:

  bool AddChild(vkNode *child);
  bool RemoveChild(vkNode *child);

  std::vector<vkNode *> m_children;

};
