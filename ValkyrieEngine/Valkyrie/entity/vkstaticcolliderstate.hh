#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/entity/vkbasecolliderstate.hh>
#include <Valkyrie/entity/vkstaticcolliderstate.refl.hh>

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
