#pragma once

#include <physicsbullet/bulletexport.hh>
#include <valkyrie/physics/iphysicsdynamiccollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <physicsbullet/bulletdynamiccollider.refl.hh>

class btRigidBody;
class btCollisionShape;
class btCompoundShape;
class vkBulletScene;
class vkBulletJoint;


VK_CLASS()
class VKBULLETPHYSICS_API vkBulletDynamicCollider : public VK_SUPER(iPhysicsDynamicCollider)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkBulletDynamicCollider();
  virtual ~vkBulletDynamicCollider();

  /**
   * \name iPhysicsCollider interface
   * @{
   */
  virtual vkPhysicsColliderType GetType() const;

  virtual const vkMatrix4f &GetMatrix() const;
  virtual vkTransformation GetTransform();
  virtual void FinishTransformation();
  virtual void SetTransformationCallback(iTransformationCallback *callback);


  virtual void AttachShape(iPhysicsShape *shape);
  virtual void DetachShape(iPhysicsShape *shape);

  virtual void AttachShape(vkPhysicsShapeContainer *shapes);
  virtual void DetachShape(vkPhysicsShapeContainer *shapes);

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

  virtual void SetInertia(const vkVector3f &inertia);
  virtual const vkVector3f &GetInertia() const;

  virtual void SetAutoInertia(bool autoInertia);
  virtual bool IsAutoInertia() const;

  /**
   * @}
   */


  void AttachToScene(vkBulletScene *scene);
  void DetachFromScene(vkBulletScene *scene);

  void PropagateTransformation();

  btRigidBody *GetRigidBody();
  const btRigidBody *GetRigidBody() const;

  void AddJoint(vkBulletJoint *joint);
  void RemoveJoint(vkBulletJoint *joint);

private:
  class MotionState : public btMotionState
  {
  public:
    vkBulletDynamicCollider *m_parent;

    virtual void	getWorldTransform(btTransform& worldTrans) const;

    //Bullet only calls the update of worldtransform for active objects
    virtual void	setWorldTransform(const btTransform& worldTrans);
  } m_motionState;


  void UpdateInertia();
  std::vector<iPhysicsShape*> m_shapes;

  iTransformationCallback *m_transformationCallback;
  //vkDynamicColliderState *m_dynamicColliderState;
  vkMatrix4f m_transformation;

  float m_friction;
  float m_restitution;

  bool m_kinematic;
  float m_mass;
  bool m_autoInertia;
  vkVector3f m_inertia;


  vkBulletScene *m_scene;
  btRigidBody *m_body;
  btCollisionShape *m_shape;
  btCompoundShape *m_compoundShape;

  void DetachJoints(vkBulletScene *scene);
  std::vector<vkBulletJoint*> m_joints;
};

VK_FORCEINLINE vkPhysicsColliderType vkBulletDynamicCollider::GetType() const
{
  return ePCT_Dynamic;
}

VK_FORCEINLINE const vkMatrix4f &vkBulletDynamicCollider::GetMatrix() const
{
  return m_transformation;
}

VK_FORCEINLINE float vkBulletDynamicCollider::GetFriction() const
{
  return m_friction;
}

VK_FORCEINLINE float vkBulletDynamicCollider::GetRestitution() const
{
  return m_restitution;
}

VK_FORCEINLINE bool vkBulletDynamicCollider::IsKinematic() const
{
  return m_kinematic;
}

VK_FORCEINLINE float vkBulletDynamicCollider::GetMass() const
{
  return m_mass;
}

VK_FORCEINLINE const vkVector3f &vkBulletDynamicCollider::GetInertia() const
{
  return m_inertia;
}

VK_FORCEINLINE bool vkBulletDynamicCollider::IsAutoInertia() const
{
  return m_autoInertia;
}



VK_FORCEINLINE btRigidBody *vkBulletDynamicCollider::GetRigidBody()
{
  return m_body;
}

VK_FORCEINLINE const btRigidBody *vkBulletDynamicCollider::GetRigidBody() const
{
  return m_body;
}
