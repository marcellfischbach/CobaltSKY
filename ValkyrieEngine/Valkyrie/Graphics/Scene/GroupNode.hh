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

protected:
  virtual void PrivScan(const vkClipper *clipper, IRenderer *renderer, IScanCallback *callback);

private:

  bool AddChild(vkNode *child);
  bool RemoveChild(vkNode *child);

  std::vector<vkNode *> m_children;

};