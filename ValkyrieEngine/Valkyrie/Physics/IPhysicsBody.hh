#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Physics/IPhysicsBody.refl.hh>
#include <Valkyrie/Entity/Transformation.hh>


struct IPhysShape;
class vkEntity;

VK_INTERFACE()
struct VKE_API IPhysicsBody : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsBody () { }

  virtual void SetEntity(vkEntity *entity) = 0;
  virtual vkEntity *GetEntity() = 0;
  virtual const vkEntity *GetEntity() const = 0;

  virtual void SetMode(vkPhysBodyMode mode) = 0;
  virtual vkPhysBodyMode GetMode() const = 0;

  virtual void SetMass(float mass) = 0;
  virtual float GetMass() const = 0;

  virtual void SetInertia(const vkVector3f &inertia) = 0;
  virtual const vkVector3f &GetInertia() const = 0;

  virtual vkTransformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;

  virtual void AttachShape(IPhysShape *shape) = 0;
  virtual void DetachShape(IPhysShape *shape) = 0;

};