#pragma once

#include <PhysicsBullet/Export.hh>
#include <Valkyrie/Physics/IPhysicsCollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <PhysicsBullet/BulletStaticCollider.refl.hh>

class vkBulletScene;
class vkBulletShape;
class btRigidBody;
class btCollisionShape;
class btCompoundShape;

VK_CLASS()
class VKBULLET_API vkBulletStaticCollider : public VK_SUPER(IPhysicsStaticCollider)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkBulletStaticCollider();
  virtual ~vkBulletStaticCollider();

  /**
  * \name IPhysicsCollider interface
  * @{
  */
  virtual vkPhysicsColliderType GetType() const;

  virtual const vkMatrix4f &GetMatrix() const;
  virtual vkTransformation GetTransform();
  virtual void FinishTransformation();

  virtual void AttachShape(IPhysicsShape *shape);
  virtual void DetachShape(IPhysicsShape *shape);

  virtual void AttachShape(vkPhysicsShapeContainer *shapes);
  virtual void DetachShape(vkPhysicsShapeContainer *shapes);

  /**
  * @}
  */

  /**
  * \name IPhysicsBaseCollider interface
  * @{
  */
  virtual void SetFriction(float friction);
  virtual float GetFriction() const;

  virtual void SetRestitution(float restitution);
  virtual float GetRestitution() const;

  /**
  * @}
  */

  void AttachToScene(vkBulletScene *scene);
  void DetachFromScene(vkBulletScene *scene);


private:

  vkMatrix4f m_transformation;

  float m_friction;
  float m_restitution;

  struct Data
  {
    Data()
    {
      localTransform.SetIdentity();
    }
    vkBulletShape *shape;
    btCollisionShape *btShape;
    btCollisionObject *object;
    vkMatrix4f localTransform;
  };

  std::vector<Data> m_shapes;
  vkBulletScene *m_scene;

};


VK_FORCEINLINE vkPhysicsColliderType vkBulletStaticCollider::GetType() const
{
  return ePCT_Static;
}

VK_FORCEINLINE const vkMatrix4f &vkBulletStaticCollider::GetMatrix() const
{
  return m_transformation;
}

VK_FORCEINLINE float vkBulletStaticCollider::GetFriction() const
{
  return m_friction;
}

VK_FORCEINLINE float vkBulletStaticCollider::GetRestitution() const
{
  return m_restitution;
}


