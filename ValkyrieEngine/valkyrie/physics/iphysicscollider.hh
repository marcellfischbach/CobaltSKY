#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/math/csmatrix.hh>
#include <valkyrie/math/csvector.hh>
#include <valkyrie/entity/itransformationcallback.hh>
#include <valkyrie/entity/cstransformation.hh>
#include <valkyrie/physics/iphysicscollider.refl.hh>


class csDynamicColliderState;
class csPhysicsShapeContainer;
struct iPhysicsShape;
class csEntity;


CS_INTERFACE()
struct CSE_API iPhysicsCollider : public iObject
{
  CS_CLASS_GEN;

  virtual ~iPhysicsCollider () { }

  virtual csPhysicsColliderType GetType() const = 0;

  virtual const csMatrix4f &GetMatrix() const = 0;
  virtual csTransformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;


  virtual void AttachShape(iPhysicsShape *shape) = 0;
  virtual void DetachShape(iPhysicsShape *shape) = 0;

  virtual void AttachShape(csPhysicsShapeContainer *shapes) = 0;
  virtual void DetachShape(csPhysicsShapeContainer *shapes) = 0;

};
