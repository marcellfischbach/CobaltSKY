#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Physics/IPhysicsCollider.refl.hh>
#include <Valkyrie/Entity/Transformation.hh>


class vkDynamicColliderState;
class vkPhysicsShapeContainer;
struct IPhysicsShape;
class vkEntity;


VK_INTERFACE()
struct VKE_API IPhysicsCollider : public IObject
{
  VK_CLASS_GEN;

  virtual ~IPhysicsCollider () { }

  virtual const vkMatrix4f &GetMatrix() const = 0;
  virtual vkTransformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;


  virtual void AttachShape(IPhysicsShape *shape) = 0;
  virtual void DetachShape(IPhysicsShape *shape) = 0;

  virtual void AttachShape(vkPhysicsShapeContainer *shapes) = 0;
  virtual void DetachShape(vkPhysicsShapeContainer *shapes) = 0;

};

VK_INTERFACE()
struct VKE_API IPhysicsTriggerCollider : public IPhysicsCollider
{
  VK_CLASS_GEN;
  virtual ~IPhysicsTriggerCollider() { }

};

VK_INTERFACE()
struct VKE_API IPhysicsBaseCollider : public IPhysicsCollider
{
  VK_CLASS_GEN;
  virtual ~IPhysicsBaseCollider() { }

  virtual void SetFriction(float friction) = 0;
  virtual float GetFriction() const = 0;

  virtual void SetRestitution(float restitution) = 0;
  virtual float GetRestitution() const = 0;

};

VK_INTERFACE()
struct VKE_API IPhysicsDynamicCollider  : public IPhysicsBaseCollider
{
  VK_CLASS_GEN;
  virtual ~IPhysicsDynamicCollider () { }

  virtual void SetTransformationCallback(ITransformationCallback *callback) = 0;

  virtual void SetKinematic(bool kinematic) = 0;
  virtual bool IsKinematic() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const vkVector3f &inertia) = 0;
  virtual const vkVector3f &GetInertia() const = 0;

  virtual void SetAutoInertia(bool autoInertia) = 0;
  virtual bool IsAutoInertia() const = 0;

};

VK_INTERFACE()
struct VKE_API IPhysicsStaticCollider : public IPhysicsBaseCollider
{
  VK_CLASS_GEN;
  virtual ~IPhysicsStaticCollider() { }

};