#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/vkentitystate.hh>
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/entity/vkcolliderstate.refl.hh>


struct iPhysicsShape;
struct iPhysicsCollider;
class vkPhysicsShapeContainer;


VK_INTERFACE()
class VKE_API vkColliderState : public VK_SUPER(vkSpatialState)
{
  VK_CLASS_GEN;
public:
  virtual ~vkColliderState();


  void AttachShape(iPhysicsShape *shape);
  void DetachShape(iPhysicsShape *shape);

  void AttachShape(vkPhysicsShapeContainer *shapes);
  void DetachShape(vkPhysicsShapeContainer *shapes);

  iPhysicsCollider *GetCollider();
  const iPhysicsCollider *GetCollider() const;


protected:
  vkColliderState();

  virtual void UpdateTransformation();

  void SetCollider(iPhysicsCollider *collider);

  bool m_updateTransformationGuard;

private:
  iPhysicsCollider *m_collider;

};


VK_FORCEINLINE iPhysicsCollider *vkColliderState::GetCollider()
{
  return m_collider;
}

VK_FORCEINLINE const iPhysicsCollider *vkColliderState::GetCollider() const
{
  return m_collider;
}

