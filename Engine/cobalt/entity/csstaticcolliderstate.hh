#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/csbasecolliderstate.hh>
#include <cobalt/entity/csstaticcolliderstate.refl.hh>



namespace cs
{
struct iPhysicsStaticCollider;

CS_CLASS()
class CSE_API StaticColliderState : public CS_SUPER(cs::BaseColliderState)
{
  CS_CLASS_GEN;
public:
  StaticColliderState();
  virtual ~StaticColliderState();

  cs::iPhysicsStaticCollider* GetStaticCollider();
  const cs::iPhysicsStaticCollider* GetStaticCollider() const;

protected:
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

private:
  cs::iPhysicsStaticCollider* m_staticCollider;

};

}

CS_FORCEINLINE cs::iPhysicsStaticCollider *cs::StaticColliderState::GetStaticCollider()
{
  return m_staticCollider;
}

CS_FORCEINLINE const cs::iPhysicsStaticCollider *cs::StaticColliderState::GetStaticCollider() const
{
  return m_staticCollider;
}
