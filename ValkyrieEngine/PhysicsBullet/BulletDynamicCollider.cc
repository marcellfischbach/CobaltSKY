
#include <PhysicsBullet/BulletDynamicCollider.hh>
#include <PhysicsBullet/BulletShape.hh>
#include <PhysicsBullet/BulletScene.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Entity/RigidBodyState.hh>

vkBulletDynamicCollider::vkBulletDynamicCollider()
  : IPhysicsDynamicCollider()
  , m_dynamicColliderState(0)
  , m_kinematic(true)
  , m_friction(0.0f)
  , m_restitution(0.0f)
  , m_mass(0.0f)
  , m_inertia(0.0f, 0.0f, 0.0f)
  , m_autoInertia(true)
  , m_body(0)
  , m_shape(0)
  , m_compoundShape(0)
{
  VK_CLASS_GEN_CONSTR;

  m_motionState.m_parent = this;

  btRigidBody::btRigidBodyConstructionInfo info(0.0f, &m_motionState, 0, btVector3(0.0f, 0.0f, 0.0f));
  m_body = new btRigidBody(info);
  m_body->setFlags(m_body->getFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
}


vkBulletDynamicCollider::~vkBulletDynamicCollider()
{
  if (m_body)
  {
    delete m_body;
  }
}


void vkBulletDynamicCollider::AttachShape(IPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  // take ownership of the shape
  m_shapes.push_back(shape);
  shape->AddRef();

  vkBulletShape *btShape = static_cast<vkBulletShape*>(shape);


  // now check if we just have to attach the shape or if this is already a compound shape
  if (!m_shape && !m_compoundShape && !btShape->IsTransformed())
  {
    m_shape = btShape->GetBulletShape();
    m_body->setCollisionShape(m_shape);
  }
  else
  {
    if (!m_compoundShape)
    {
      m_compoundShape = new btCompoundShape();
      m_body->setCollisionShape(m_compoundShape);
    }

    if (m_shape)
    {
      btTransform trans;
      trans.setIdentity();
      m_compoundShape->addChildShape(trans, m_shape);
      m_shape = 0;
    }

    btTransform trans;
    trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&shape->GetLocalTransform().m00));
    m_compoundShape->addChildShape(trans, btShape->GetBulletShape());
  }

  UpdateInertia();
 }

void vkBulletDynamicCollider::DetachShape(IPhysicsShape *shape)
{
  // not implemented yet
}

void vkBulletDynamicCollider::AttachShape(vkPhysicsShapeContainer *shapes)
{
  if (!shapes)
  {
    return;
  }

  for (vkSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    AttachShape(shapes->GetShape(i));
  }
}

void vkBulletDynamicCollider::DetachShape(vkPhysicsShapeContainer *shapes)
{
  if (!shapes)
  {
    return;
  }

  for (vkSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    DetachShape(shapes->GetShape(i));
  }
}


vkTransformation vkBulletDynamicCollider::GetTransform()
{
  return vkTransformation(&m_transformation, 0, 0);
}


void vkBulletDynamicCollider::FinishTransformation()
{
  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_transformation.m00));
  m_body->setWorldTransform(trans);
}

void vkBulletDynamicCollider::SetDynamicColliderState(vkDynamicColliderState *colliderState)
{
  m_dynamicColliderState = colliderState;
}

void vkBulletDynamicCollider::SetKinematic(bool kinematic)
{
  m_kinematic = kinematic;

  if (m_kinematic)
  {
    m_body->setFlags(m_body->getFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
  }
  else
  {
    m_body->setFlags(m_body->getFlags() & ~btRigidBody::CF_KINEMATIC_OBJECT);
  }
}

void vkBulletDynamicCollider::SetFriction(float friction)
{
  m_friction = friction;
  m_body->setFriction(friction);
}

void vkBulletDynamicCollider::SetRestitution(float restitution)
{
  m_restitution = restitution;
  m_body->setRestitution(restitution);
}


void vkBulletDynamicCollider::SetMass(float mass)
{
  m_mass = mass;

  UpdateInertia();
}

void vkBulletDynamicCollider::SetInertia(const vkVector3f &inertia)
{
  m_inertia = inertia;
  m_autoInertia = false;

  UpdateInertia();
}

void vkBulletDynamicCollider::SetAutoInertia(bool autoInertia)
{
  m_autoInertia = autoInertia;

  UpdateInertia();
}

void vkBulletDynamicCollider::UpdateInertia()
{
  if (m_autoInertia)
  {
    btCollisionShape *shape = m_shape ? m_shape : m_compoundShape;
    if (shape)
    {
      btVector3 btInertia;
      shape->calculateLocalInertia(m_mass, btInertia);
      m_inertia = vkVector3f(btInertia.getX(), btInertia.getY(), btInertia.getZ());
    }
    else
    {
      m_inertia = vkVector3f(0.0f, 0.0f, 0.0f);
    }
  }

  btVector3 inertia(m_inertia.x, m_inertia.y, m_inertia.z);
  m_body->setMassProps(m_mass, inertia);
}


void vkBulletDynamicCollider::AttachToScene(vkBulletScene *scene)
{
  m_scene = scene;
  if (m_scene && m_scene->GetBulletScene())
  {
    m_scene->GetBulletScene()->addRigidBody(m_body);
  }
}

void vkBulletDynamicCollider::DetachFromScene(vkBulletScene *scene)
{
;
  if (m_scene && m_scene->GetBulletScene())
  {
    m_scene->GetBulletScene()->removeRigidBody(m_body);
  }
  m_scene = 0;
}



void vkBulletDynamicCollider::MotionState::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_parent->m_transformation.m00));
}

void vkBulletDynamicCollider::MotionState::setWorldTransform(const btTransform& worldTrans)
{
  worldTrans.getOpenGLMatrix(static_cast<btScalar*>(&m_parent->m_transformation.m00));

  if (m_parent->m_scene)
  {
    m_parent->m_scene->DynamicColliderChanged(m_parent);
  }
  // inform bullet system that this dynamic collider has changed
}

void vkBulletDynamicCollider::UpdateColliderState()
{
  if (m_dynamicColliderState)
  {
    m_dynamicColliderState->DynamicTransformationChanged(m_transformation);
  }
}