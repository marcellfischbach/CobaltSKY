#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/physics/iphysicsdynamiccollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <physicsbullet/bulletdynamiccollider.refl.hh>

class btRigidBody;
class btCollisionShape;
class btCompoundShape;


namespace cs
{
class BulletScene;
class BulletJoint;

CS_CLASS()
class CSBULLETPHYSICS_API BulletDynamicCollider : public CS_SUPER(cs::iPhysicsDynamicCollider)
{
  CS_CLASS_GEN_OBJECT;

public:
  BulletDynamicCollider();
  virtual ~BulletDynamicCollider();

  /**
   * \name cs::iPhysicsCollider interface
   * @{
   */
  virtual cs::ePhysicsColliderType GetType() const;

  virtual const cs::Matrix4f& GetMatrix() const;
  virtual cs::Transformation GetTransform();
  virtual void FinishTransformation();
  virtual void SetTransformationCallback(cs::iTransformationCallback * callback);


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

   /**
    * \name cs::iPhysicsDynamicCollider interface
    * @{
    */

  virtual void SetKinematic(bool kinematic);
  virtual bool IsKinematic() const;

  virtual void SetMass(float mass);
  virtual float GetMass() const;

  virtual void SetInertia(const cs::Vector3f & inertia);
  virtual const cs::Vector3f& GetInertia() const;

  virtual void SetAutoInertia(bool autoInertia);
  virtual bool IsAutoInertia() const;

  /**
   * @}
   */


  void AttachToScene(cs::BulletScene * scene);
  void DetachFromScene(cs::BulletScene * scene);

  void PropagateTransformation();

  btRigidBody* GetRigidBody();
  const btRigidBody* GetRigidBody() const;

  void AddJoint(cs::BulletJoint * joint);
  void RemoveJoint(cs::BulletJoint * joint);

private:
  class MotionState : public btMotionState
  {
  public:
    cs::BulletDynamicCollider* m_parent;

    virtual void	getWorldTransform(btTransform& worldTrans) const;

    //Bullet only calls the update of worldtransform for active objects
    virtual void	setWorldTransform(const btTransform& worldTrans);
  } m_motionState;


  void UpdateInertia();
  std::vector<cs::iPhysicsShape*> m_shapes;

  cs::iTransformationCallback* m_transformationCallback;
  //cs::DynamicColliderState *m_dynamicColliderState;
  cs::Matrix4f m_transformation;

  float m_friction;
  float m_restitution;

  bool m_kinematic;
  float m_mass;
  bool m_autoInertia;
  cs::Vector3f m_inertia;


  cs::BulletScene* m_scene;
  btRigidBody* m_body;
  btCollisionShape* m_shape;
  btCompoundShape* m_compoundShape;

  void DetachJoints(cs::BulletScene * scene);
  std::vector<cs::BulletJoint*> m_joints;
};

}

CS_FORCEINLINE cs::ePhysicsColliderType cs::BulletDynamicCollider::GetType() const
{
  return cs::ePCT_Dynamic;
}

CS_FORCEINLINE const cs::Matrix4f &cs::BulletDynamicCollider::GetMatrix() const
{
  return m_transformation;
}

CS_FORCEINLINE float cs::BulletDynamicCollider::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float cs::BulletDynamicCollider::GetRestitution() const
{
  return m_restitution;
}

CS_FORCEINLINE bool cs::BulletDynamicCollider::IsKinematic() const
{
  return m_kinematic;
}

CS_FORCEINLINE float cs::BulletDynamicCollider::GetMass() const
{
  return m_mass;
}

CS_FORCEINLINE const cs::Vector3f &cs::BulletDynamicCollider::GetInertia() const
{
  return m_inertia;
}

CS_FORCEINLINE bool cs::BulletDynamicCollider::IsAutoInertia() const
{
  return m_autoInertia;
}



CS_FORCEINLINE btRigidBody *cs::BulletDynamicCollider::GetRigidBody()
{
  return m_body;
}

CS_FORCEINLINE const btRigidBody *cs::BulletDynamicCollider::GetRigidBody() const
{
  return m_body;
}

