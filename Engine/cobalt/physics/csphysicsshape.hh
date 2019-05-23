#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <vector>
#include <cobalt/physics/csphysicsshape.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API PhysicsShape : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  PhysicsShape();
  virtual ~PhysicsShape();

  void AddShape(cs::iPhysicsShape * shape);
  void RemoveShape(csSize idx);

  csSize GetNumberOfShapes() const;
  cs::iPhysicsShape* GetShape(csSize idx);
  const cs::iPhysicsShape* GetShape(csSize idx) const;

private:
  std::vector<cs::iPhysicsShape*> m_shapes;

};

CS_CLASS()
class CSE_API PhysicsShapeWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::PhysicsShape, PhysicsShapeWrapper, cs::ResourceWrapper);
};

}