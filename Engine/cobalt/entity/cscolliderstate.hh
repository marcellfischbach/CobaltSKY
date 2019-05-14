#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/csentitystate.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/csenums.hh>
#include <cobalt/entity/cscolliderstate.refl.hh>


struct iPhysicsShape;
struct iPhysicsCollider;
class csPhysicsShape;


CS_CLASS()
class CSE_API csColliderState : public CS_SUPER(csSpatialState)
{
  CS_CLASS_GEN;
public:
  virtual ~csColliderState();


  void AttachShape(iPhysicsShape *shape);
  void DetachShape(iPhysicsShape *shape);

  void AttachShape(csPhysicsShape *shapes);
  void DetachShape(csPhysicsShape *shapes);

  iPhysicsCollider *GetCollider();
  const iPhysicsCollider *GetCollider() const;


protected:
  csColliderState();

  virtual void UpdateTransformation();

  void SetCollider(iPhysicsCollider *collider);

  bool m_updateTransformationGuard;

private:
  iPhysicsCollider *m_collider;

};


CS_FORCEINLINE iPhysicsCollider *csColliderState::GetCollider()
{
  return m_collider;
}

CS_FORCEINLINE const iPhysicsCollider *csColliderState::GetCollider() const
{
  return m_collider;
}

