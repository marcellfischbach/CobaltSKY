#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/vkentitystate.hh>
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/entity/vkcolliderstate.refl.hh>


struct IPhysicsShape;
struct IPhysicsCollider;
class vkPhysicsShapeContainer;


VK_INTERFACE()
class VKE_API vkColliderState : public VK_SUPER(vkSpatialState)
{
  VK_CLASS_GEN;
public:
  virtual ~vkColliderState();


  void AttachShape(IPhysicsShape *shape);
  void DetachShape(IPhysicsShape *shape);

  void AttachShape(vkPhysicsShapeContainer *shapes);
  void DetachShape(vkPhysicsShapeContainer *shapes);

  IPhysicsCollider *GetCollider();
  const IPhysicsCollider *GetCollider() const;


protected:
  vkColliderState();

  virtual void UpdateTransformation();

  void SetCollider(IPhysicsCollider *collider);

  bool m_updateTransformationGuard;

private:
  IPhysicsCollider *m_collider;

};


VK_FORCEINLINE IPhysicsCollider *vkColliderState::GetCollider()
{
  return m_collider;
}

VK_FORCEINLINE const IPhysicsCollider *vkColliderState::GetCollider() const
{
  return m_collider;
}

