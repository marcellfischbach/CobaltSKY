#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicsdynamiccollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <physicsbullet/bulletdynamiccollider.refl.hh>

class btRigidBody;
class btCollisionShape;
class btCompoundShape;
class csBulletScene;
class csBulletJoint;


CS_CLASS()
class CSBULLETPHYSICS_API csBulletDynamicCollider : public CS_SUPER(iPhysicsDynamicCollider)
{
  CS_CLASS_GEN_OBJECT;

public:
  csBulletDynamicCollider();
  virtual ~csBulletDynamicCollider();

  /**
   * \name iPhysicsCollider interface
   * @{
   */
  virtual csPhysicsColliderType GetType() const;

  virtual const csMatrix4f &GetMatrix() const;
  virtual csTransformation GetTransform();
  virtual void FinishTransformation();
  virtual void SetTransformationCallback(iTransformationCallback *callback);


  virtual void AttachShape(iPhysicsShape *shape);
  virtual void DetachShape(iPhysicsShape *shape);

  virtual void AttachShape(csPhysicsShapeContainer *shapes);
  virtual void DetachShape(csPhysicsShapeContainer *shapes);

  /**
   * @}
   */

   /**
    * \name iPhysicsBaseCollider interface
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
    * \name iPhysicsDynamicCollider interface
    * @{
    */

  virtual void SetKinematic(bool kinematic);
  virtual bool IsKinematic() const;

  virtual void SetMass(float mass);
  virtual float GetMass() const;

  virtual void SetInertia(const csVector3f &inertia);
  virtual const csVector3f &GetInertia() const;

  virtual void SetAutoInertia(bool autoInertia);
  virtual bool IsAutoInertia() const;

  /**
   * @}
   */


  void AttachToScene(csBulletScene *scene);
  void DetachFromScene(csBulletScene *scene);

  void PropagateTransformation();

  btRigidBody *GetRigidBody();
  const btRigidBody *GetRigidBody() const;

  void AddJoint(csBulletJoint *joint);
  void RemoveJoint(csBulletJoint *joint);

private:
  class MotionState : public btMotionState
  {
  public:
    csBulletDynamicCollider *m_parent;

    virtual void	getWorldTransform(btTransform& worldTrans) const;

    //Bullet only calls the update of worldtransform for active objects
    virtual void	setWorldTransform(const btTransform& worldTrans);
  } m_motionState;


  void UpdateInertia();
  std::vector<iPhysicsShape*> m_shapes;

  iTransformationCallback *m_transformationCallback;
  //csDynamicColliderState *m_dynamicColliderState;
  csMatrix4f m_transformation;

  float m_friction;
  float m_restitution;

  bool m_kinematic;
  float m_mass;
  bool m_autoInertia;
  csVector3f m_inertia;


  csBulletScene *m_scene;
  btRigidBody *m_body;
  btCollisionShape *m_shape;
  btCompoundShape *m_compoundShape;

  void DetachJoints(csBulletScene *scene);
  std::vector<csBulletJoint*> m_joints;
};

CS_FORCEINLINE csPhysicsColliderType csBulletDynamicCollider::GetType() const
{
  return ePCT_Dynamic;
}

CS_FORCEINLINE const csMatrix4f &csBulletDynamicCollider::GetMatrix() const
{
  return m_transformation;
}

CS_FORCEINLINE float csBulletDynamicCollider::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float csBulletDynamicCollider::GetRestitution() const
{
  return m_restitution;
}

CS_FORCEINLINE bool csBulletDynamicCollider::IsKinematic() const
{
  return m_kinematic;
}

CS_FORCEINLINE float csBulletDynamicCollider::GetMass() const
{
  return m_mass;
}

CS_FORCEINLINE const csVector3f &csBulletDynamicCollider::GetInertia() const
{
  return m_inertia;
}

CS_FORCEINLINE bool csBulletDynamicCollider::IsAutoInertia() const
{
  return m_autoInertia;
}



CS_FORCEINLINE btRigidBody *csBulletDynamicCollider::GetRigidBody()
{
  return m_body;
}

CS_FORCEINLINE const btRigidBody *csBulletDynamicCollider::GetRigidBody() const
{
  return m_body;
}
