#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkbasecolliderstate.hh>
#include <valkyrie/entity/vkstaticcolliderstate.refl.hh>

struct IPhysicsStaticCollider;


VK_CLASS()
class VKE_API vkStaticColliderState : public VK_SUPER(vkBaseColliderState)
{
  VK_CLASS_GEN;
public:
  vkStaticColliderState();
  virtual ~vkStaticColliderState();

  IPhysicsStaticCollider *GetStaticCollider();
  const IPhysicsStaticCollider *GetStaticCollider() const;

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsStaticCollider *m_staticCollider;

};

VK_FORCEINLINE IPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider()
{
  return m_staticCollider;
}

VK_FORCEINLINE const IPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider() const
{
  return m_staticCollider;
}
