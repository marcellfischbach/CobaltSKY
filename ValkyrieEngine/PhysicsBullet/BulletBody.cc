

#include <PhysicsBullet/BulletBody.hh>
#include <PhysicsBullet/BulletShape.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <Valkyrie/Entity/Transformation.hh>

vkBulletBody::vkBulletBody()
  : IPhysicsBody()
  , m_bodyMode(ePBM_Static)
  , m_entity(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkBulletBody::~vkBulletBody()
{

}


void vkBulletBody::SetEntity(vkEntity *entity)
{
  m_entity = entity;
}

vkEntity *vkBulletBody::GetEntity()
{
  return m_entity;
}

const vkEntity *vkBulletBody::GetEntity() const
{
  return m_entity;
}

void vkBulletBody::SetMode(vkPhysBodyMode mode)
{
  if (m_bodies.size() > 0)
  {
    // ones the first body is created it is not possible to change the mode
    return;
  }
  m_bodyMode = mode;
}

vkPhysBodyMode vkBulletBody::GetMode() const
{
  return m_bodyMode;
}

void vkBulletBody::SetMass(float mass)
{
  m_mass = mass;
  UpdateBodies();
}

float vkBulletBody::GetMass() const
{
  return m_mass;
}

void vkBulletBody::SetInertia(const vkVector3f &inertia)
{
  m_inertia = inertia;
  UpdateBodies();
}

const vkVector3f &vkBulletBody::GetInertia() const
{
  return m_inertia;
}

void vkBulletBody::UpdateBodies()
{
  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    m_bodies[i]->UpdateRigidBody();
  }
}

vkTransformation vkBulletBody::GetTransform()
{
  return vkTransformation(&m_transform, 0, 0);
}

void vkBulletBody::FinishTransformation()
{
  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    m_bodies[i]->UpdateTransform(m_transform);
  }
}

const vkMatrix4f &vkBulletBody::GetTransform() const
{
  return m_transform;
}


void vkBulletBody::AttachShape(IPhysShape *shape)
{
  vkBulletShape *btShape = reinterpret_cast<vkBulletShape*>(shape);
  if (!btShape)
  {
    return;
  }

  btCollisionShape *collShape = btShape->GetBulletShape();
  if (!collShape)
  {
    return;
  }

  if (m_bodyMode == ePBM_Dynamic)
  {
    // in dynamic mode we must work with one single rigid body 
    if (m_bodies.size() == 0)
    {
      vkBulletBodyImpl *impl = new vkBulletBodyImpl(this);
      m_bodies.push_back(impl);
    }

    m_bodies[0]->AttachShape(btShape);
  }
  else
  {
    // in static or kinematic mode we can work with multiple rigid bodies that are transformed
    // simultaniously
    vkBulletBodyImpl *impl = new vkBulletBodyImpl(this);
    m_bodies.push_back(impl);
    impl->AttachShape(btShape);
  }
}

void vkBulletBody::DetachShape(IPhysShape *shape)
{
  vkBulletShape *btShape = reinterpret_cast<vkBulletShape*>(shape);
  if (!btShape)
  {
    return;
  }

  btCollisionShape *collShape = btShape->GetBulletShape();
  if (!collShape)
  {
    return;
  }

  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    vkBulletBodyImpl *impl = m_bodies[i];
    if (impl->DetachShape(btShape))
    {
      if (!impl->HasShape())
      {
        m_bodies.erase(m_bodies.begin() + i);
        return;
      }
    }
  }
}


void vkBulletBody::DynamicallyChanged(const vkMatrix4f &transform)
{
  if (m_bodyMode == ePBM_Dynamic)
  {
    if (m_entity)
    {
      m_entity->GetTransformation().SetTransformation(transform);
      m_entity->FinishTransformation();
    }
  }
}





vkBulletBodyImpl::vkBulletBodyImpl(vkBulletBody *body)
  : m_collisionShape(0)
  , m_compoundShape(0)
  , m_rigidBody(0)
  , m_bulletBody(body)
{

}

vkBulletBodyImpl::~vkBulletBodyImpl()
{

}


void vkBulletBodyImpl::AttachShape(vkBulletShape *shape)
{
  if (!shape)
  {
    return;
  }

  if (!m_collisionShape && !shape->IsTransformed())
  {
    m_collisionShape = shape->GetBulletShape();
  }
  else
  {
    if (!m_compoundShape)
    {
      m_compoundShape = new btCompoundShape();
    }
    btTransform trans;
    if (m_collisionShape)
    {
      trans.setIdentity();

      m_compoundShape->addChildShape(trans, m_collisionShape);
      m_collisionShape = 0;
    }

    trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&shape->GetLocalTransform().m00));
    m_compoundShape->addChildShape(trans, shape->GetBulletShape());
  }

  UpdateRigidBody();
}


bool vkBulletBodyImpl::DetachShape(vkBulletShape *shape)
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    vkBulletShape *myShape = m_shapes[i];
    if (myShape == shape)
    {
      return true;
    }
  }
  return false;
}

bool vkBulletBodyImpl::HasShape() const
{
  return m_shapes.size() > 0;
}


void vkBulletBodyImpl::UpdateRigidBody()
{
  btCollisionShape *shape = m_collisionShape ? m_collisionShape : m_compoundShape;
  if (!shape)
  {
    return;
  }
  float mass = m_bulletBody->GetMass();
  vkVector3f l_inertia = m_bulletBody->GetInertia();
  btVector3 inertia(l_inertia.x, l_inertia.y, l_inertia.z);


  if (!m_rigidBody)
  {
    btRigidBody::btRigidBodyConstructionInfo info(mass, this, shape, inertia);
    m_rigidBody = new btRigidBody(info);
  }
  else
  {
    m_rigidBody->setCollisionShape(shape);
    m_rigidBody->setMassProps(mass, inertia);
    m_rigidBody->updateInertiaTensor();
  }
}

void vkBulletBodyImpl::UpdateTransform(const vkMatrix4f &transform)
{
  if (!m_rigidBody)
  {
    return;
  }

  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&transform.m00));
  m_rigidBody->setWorldTransform(trans);

}

void vkBulletBodyImpl::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_bulletBody->GetTransform().m00));
}

void vkBulletBodyImpl::setWorldTransform(const btTransform &worldTrans)
{
  vkMatrix4f trans;
  worldTrans.getOpenGLMatrix(static_cast<btScalar*>(&trans.m00));

  m_bulletBody->DynamicallyChanged(trans);
}
