#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/entity/itransformationcallback.hh>
#include <valkyrie/entity/vktransformation.hh>
#include <valkyrie/physics/iphysicscollider.refl.hh>


class vkDynamicColliderState;
class vkPhysicsShapeContainer;
struct iPhysicsShape;
class vkEntity;


VK_INTERFACE()
struct VKE_API iPhysicsCollider : public iObject
{
  VK_CLASS_GEN;

  virtual ~iPhysicsCollider () { }

  virtual vkPhysicsColliderType GetType() const = 0;

  virtual const vkMatrix4f &GetMatrix() const = 0;
  virtual vkTransformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;


  virtual void AttachShape(iPhysicsShape *shape) = 0;
  virtual void DetachShape(iPhysicsShape *shape) = 0;

  virtual void AttachShape(vkPhysicsShapeContainer *shapes) = 0;
  virtual void DetachShape(vkPhysicsShapeContainer *shapes) = 0;

};
