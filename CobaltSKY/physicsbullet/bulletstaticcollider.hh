#pragma once

#include <physicsbullet/bulletexport.hh>
#include <cobalt/physics/iphysicsstaticcollider.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <vector>
#include <physicsbullet/bulletstaticcollider.refl.hh>

class csBulletScene;
class csBulletShape;
class btRigidBody;
class btCollisionShape;
class btCompoundShape;

CS_CLASS()
class CSBULLETPHYSICS_API csBulletStaticCollider : public CS_SUPER(iPhysicsStaticCollider)
{
  CS_CLASS_GEN_OBJECT;

public:
  csBulletStaticCollider();
  virtual ~csBulletStaticCollider();

  /**
  * \name iPhysicsCollider interface
  * @{
  */
  virtual csPhysicsColliderType GetType() const;

  virtual const csMatrix4f &GetMatrix() const;
  virtual csTransformation GetTransform();
  virtual void FinishTransformation();

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

  void AttachToScene(csBulletScene *scene);
  void DetachFromScene(csBulletScene *scene);


private:

  csMatrix4f m_transformation;

  float m_friction;
  float m_restitution;

  struct Data
  {
    Data()
    {
      localTransform.SetIdentity();
    }
    csBulletShape *shape;
    btCollisionShape *btShape;
    btCollisionObject *object;
    csMatrix4f localTransform;
  };

  std::vector<Data> m_shapes;
  csBulletScene *m_scene;

};


CS_FORCEINLINE csPhysicsColliderType csBulletStaticCollider::GetType() const
{
  return ePCT_Static;
}

CS_FORCEINLINE const csMatrix4f &csBulletStaticCollider::GetMatrix() const
{
  return m_transformation;
}

CS_FORCEINLINE float csBulletStaticCollider::GetFriction() const
{
  return m_friction;
}

CS_FORCEINLINE float csBulletStaticCollider::GetRestitution() const
{
  return m_restitution;
}


