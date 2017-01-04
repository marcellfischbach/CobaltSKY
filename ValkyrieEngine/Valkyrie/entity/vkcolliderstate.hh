#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/entity/vkentitystate.hh>
#include <Valkyrie/entity/vkspatialstate.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/entity/vkcolliderstate.refl.hh>


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

