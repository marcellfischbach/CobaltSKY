#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/vkmatrix.hh>
#include <Valkyrie/Math/vkvector.hh>
#include <Valkyrie/entity/itransformationcallback.hh>
#include <Valkyrie/entity/vktransformation.hh>
#include <Valkyrie/physics/iphysicscollider.refl.hh>


class vkDynamicColliderState;
class vkPhysicsShapeContainer;
struct IPhysicsShape;
class vkEntity;


VK_INTERFACE()
struct VKE_API IPhysicsCollider : public IObject
{
  VK_CLASS_GEN;

  virtual ~IPhysicsCollider () { }

  virtual vkPhysicsColliderType GetType() const = 0;

  virtual const vkMatrix4f &GetMatrix() const = 0;
  virtual vkTransformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;


  virtual void AttachShape(IPhysicsShape *shape) = 0;
  virtual void DetachShape(IPhysicsShape *shape) = 0;

  virtual void AttachShape(vkPhysicsShapeContainer *shapes) = 0;
  virtual void DetachShape(vkPhysicsShapeContainer *shapes) = 0;

};
