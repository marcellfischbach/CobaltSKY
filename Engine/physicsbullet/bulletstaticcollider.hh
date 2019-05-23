#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <physicsbullet/bulletstaticcollider.refl.hh>

class btRigidBody;
class btCollisionShape;
class btCompoundShape;

namespace cs
{
class BulletScene;
class BulletShape;

CS_CLASS()
class CSBULLETPHYSICS_API BulletStaticCollider : public CS_SUPER(cs::iPhysicsStaticCollider)
{
  CS_CLASS_GEN_OBJECT;

public:
  BulletStaticCollider();
  virtual ~BulletStaticCollider();

  /**
  * \name cs::iPhysicsCollider interface
  * @{
  */
  virtual cs::ePhysicsColliderType GetType() const;

  virtual const cs::Matrix4f& GetMatrix() const;
  virtual cs::Transformation GetTransform();
  virtual void FinishTransformation();

  virtual void AttachShape(cs::iPhysicsShape * shape);
  virtual void DetachShape(cs::iPhysicsShape * shape);

  virtual void AttachShape(cs::PhysicsShape * shapes);
  virtual void DetachShape(cs::PhysicsShape * shapes);

  /**
  * @}
  */

  /**
  * \name cs::iPhysicsBaseCollider interface
  * @{
  */
  virtual void SetFriction(float friction);
  virtual float GetFriction() const;

  virtual void SetRestitution(float restitution);
  virtual float GetRestitution() const;

  /**
  * @}
  */

  void AttachToScene(cs::BulletScene * scene);
  void DetachFromScene(cs::BulletScene * scene);


private:

  cs::Matrix4f m_transformation;

  float m_friction;
  float m_restitution;

  struct Data
  {
    Data()
    {
      localTransform.SetIdentity();
    }
    cs::BulletShape* shape;
    btCollisionShape* btShape;
    btCollisionObject* object;
    cs::Matrix4f localTransform;
  };

  std::vector<Data> m_shapes;
  cs::BulletScene* m_scene;

};

}

CS_FORCEINLINE cs::ePhysicsColliderType cs::BulletStaticCollider::GetType() const
{
  return cs::ePCT_Static;
}

CS_FORCEINLINE const cs::Matrix4f &cs::BulletStaticCollider::GetMatrix() const
{
  return m_transformation;
}

CS_FORCEINLINE float cs::BulletStaticCollider::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float cs::BulletStaticCollider::GetRestitution() const
{
  return m_restitution;
}


