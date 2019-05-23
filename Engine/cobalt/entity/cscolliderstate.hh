#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/entity/cscolliderstate.refl.hh>



namespace cs
{
struct iPhysicsShape;
struct iPhysicsCollider;
class PhysicsShape;

CS_CLASS()
class CSE_API ColliderState : public CS_SUPER(cs::SpatialState)
{
  CS_CLASS_GEN;
public:
  virtual ~ColliderState();


  void AttachShape(cs::iPhysicsShape * shape);
  void DetachShape(cs::iPhysicsShape * shape);

  void AttachShape(cs::PhysicsShape * shapes);
  void DetachShape(cs::PhysicsShape * shapes);

  cs::iPhysicsCollider* GetCollider();
  const cs::iPhysicsCollider* GetCollider() const;


protected:
  ColliderState();

  virtual void UpdateTransformation();

  void SetCollider(cs::iPhysicsCollider * collider);

  bool m_updateTransformationGuard;

private:
  cs::iPhysicsCollider* m_collider;

};

}


CS_FORCEINLINE cs::iPhysicsCollider *cs::ColliderState::GetCollider()
{
  return m_collider;
}

CS_FORCEINLINE const cs::iPhysicsCollider *cs::ColliderState::GetCollider() const
{
  return m_collider;
}

