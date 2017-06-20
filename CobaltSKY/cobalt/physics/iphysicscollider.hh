#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/physics/iphysicscollider.refl.hh>


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
