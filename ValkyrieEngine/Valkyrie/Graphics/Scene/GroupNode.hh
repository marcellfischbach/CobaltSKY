#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/Scene/Node.hh>
#include <Valkyrie/Graphics/Scene/GroupNode.refl.hh>
#include <vector>

VK_CLASS();
class VKE_API vkGroupNode : public vkNode
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