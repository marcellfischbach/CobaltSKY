#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/csbasecolliderstate.hh>
#include <cobalt/entity/csstaticcolliderstate.refl.hh>

struct iPhysicsStaticCollider;


CS_CLASS()
class CSE_API csStaticColliderState : public CS_SUPER(csBaseColliderState)
{
  CS_CLASS_GEN;
public:
  csStaticColliderState();
  virtual ~csStaticColliderState();

  iPhysicsStaticCollider *GetStaticCollider();
  const iPhysicsStaticCollider *GetStaticCollider() const;

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

private:
  iPhysicsStaticCollider *m_staticCollider;

};

CS_FORCEINLINE iPhysicsStaticCollider *csStaticColliderState::GetStaticCollider()
{
  return m_staticCollider;
}

CS_FORCEINLINE const iPhysicsStaticCollider *csStaticColliderState::GetStaticCollider() const
{
  return m_staticCollider;
}
