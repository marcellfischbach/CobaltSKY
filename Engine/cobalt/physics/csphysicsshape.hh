#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <vector>
#include <cobalt/physics/csphysicsshape.refl.hh>

CS_CLASS()
class CSE_API csPhysicsShape : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csPhysicsShape();
  virtual ~csPhysicsShape();

  void AddShape(iPhysicsShape *shape);
  void RemoveShape(csSize idx);

  csSize GetNumberOfShapes() const;
  iPhysicsShape *GetShape(csSize idx);
  const iPhysicsShape *GetShape(csSize idx) const;

private:
  std::vector<iPhysicsShape*> m_shapes;

};

