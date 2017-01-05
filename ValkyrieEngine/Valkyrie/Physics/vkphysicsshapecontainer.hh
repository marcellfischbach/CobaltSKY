#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Physics/iphysicsshape.hh>
#include <vector>
#include <Valkyrie/Physics/vkphysicsshapecontainer.refl.hh>

VK_CLASS()
class VKE_API vkPhysicsShapeContainer : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkPhysicsShapeContainer();
  virtual ~vkPhysicsShapeContainer();

  void AddShape(IPhysicsShape *shape);
  void RemoveShape(vkSize idx);

  vkSize GetNumberOfShapes() const;
  IPhysicsShape *GetShape(vkSize idx);
  const IPhysicsShape *GetShape(vkSize idx) const;

private:
  std::vector<IPhysicsShape*> m_shapes;

};

