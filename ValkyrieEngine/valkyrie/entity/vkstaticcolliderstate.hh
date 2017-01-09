#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkbasecolliderstate.hh>
#include <valkyrie/entity/vkstaticcolliderstate.refl.hh>

struct iPhysicsStaticCollider;


VK_CLASS()
class VKE_API vkStaticColliderState : public VK_SUPER(vkBaseColliderState)
{
  VK_CLASS_GEN;
public:
  vkStaticColliderState();
  virtual ~vkStaticColliderState();

  iPhysicsStaticCollider *GetStaticCollider();
  const iPhysicsStaticCollider *GetStaticCollider() const;

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  iPhysicsStaticCollider *m_staticCollider;

};

VK_FORCEINLINE iPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider()
{
  return m_staticCollider;
}

VK_FORCEINLINE const iPhysicsStaticCollider *vkStaticColliderState::GetStaticCollider() const
{
  return m_staticCollider;
}
