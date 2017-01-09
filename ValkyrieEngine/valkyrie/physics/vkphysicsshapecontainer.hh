#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/physics/iphysicsshape.hh>
#include <vector>
#include <valkyrie/physics/vkphysicsshapecontainer.refl.hh>

VK_CLASS()
class VKE_API vkPhysicsShapeContainer : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkPhysicsShapeContainer();
  virtual ~vkPhysicsShapeContainer();

  void AddShape(iPhysicsShape *shape);
  void RemoveShape(vkSize idx);

  vkSize GetNumberOfShapes() const;
  iPhysicsShape *GetShape(vkSize idx);
  const iPhysicsShape *GetShape(vkSize idx) const;

private:
  std::vector<iPhysicsShape*> m_shapes;

};

