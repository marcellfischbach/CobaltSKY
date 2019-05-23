#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/physics/iphysicscollider.refl.hh>




namespace cs
{
struct iPhysicsShape;
class Entity;
class DynamicColliderState;
class PhysicsShape;



CS_CLASS()
struct CSE_API iPhysicsCollider : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iPhysicsCollider() { }

  virtual cs::ePhysicsColliderType GetType() const = 0;

  virtual const cs::Matrix4f& GetMatrix() const = 0;
  virtual cs::Transformation GetTransform() = 0;
  virtual void FinishTransformation() = 0;


  virtual void AttachShape(cs::iPhysicsShape* shape) = 0;
  virtual void DetachShape(cs::iPhysicsShape* shape) = 0;

  virtual void AttachShape(cs::PhysicsShape* shapes) = 0;
  virtual void DetachShape(cs::PhysicsShape* shapes) = 0;

};

}