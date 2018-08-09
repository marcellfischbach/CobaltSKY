
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bulletshape.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletjoint.hh>
#include <cobalt/physics/csphysicsshape.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/entity/cscolliderstate.hh>

csBulletDynamicCollider::csBulletDynamicCollider()
  : iPhysicsDynamicCollider()
  , m_kinematic(true)
  , m_friction(0.0f)
  , m_restitution(0.0f)
  , m_mass(0.0f)
  , m_inertia(0.0f, 0.0f, 0.0f)
  , m_autoInertia(true)
  , m_body(0)
  , m_shape(0)
  , m_compoundShape(0)
  , m_transformationCallback(0)
{
  CS_CLASS_GEN_CONSTR;

  m_motionState.m_parent = this;

  btRigidBody::btRigidBodyConstructionInfo info(0.0f, &m_motionState, 0, btVector3(0.0f, 0.0f, 0.0f));
  m_body = new btRigidBody(info);
  m_body->setFlags(m_body->getFlags() | btRigidBody::CF_KINEMATIC_OBJECT);
}


csBulletDynamicCollider::~csBulletDynamicCollider()
{
  if (m_body)
  {
    delete m_body;
  }
}


void csBulletDynamicCollider::AttachShape(iPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  // take ownership of the shape
  m_shapes.push_back(shape);
  shape->AddRef();

  csBulletShape *btShape = static_cast<csBulletShape*>(shape);


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

void csBulletDynamicCollider::DetachShape(iPhysicsShape *shape)
{
  // not implemented yet
}

void csBulletDynamicCollider::AttachShape(csPhysicsShape *shapes)
{
  if (!shapes)
  {
    return;
  }

  for (csSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    AttachShape(shapes->GetShape(i));
  }
}

void csBulletDynamicCollider::DetachShape(csPhysicsShape *shapes)
{
  if (!shapes)
  {
    return;
  }

  for (csSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    DetachShape(shapes->GetShape(i));
  }
}


csTransformation csBulletDynamicCollider::GetTransform()
{
  return csTransformation(&m_transformation, 0, 0, 0);
}


void csBulletDynamicCollider::FinishTransformation()
{
  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_transformation.m00));
  m_body->setWorldTransform(trans);
  m_body->updateInertiaTensor();
}

void csBulletDynamicCollider::SetTransformationCallback(iTransformationCallback *callback)
{
  m_transformationCallback = callback;
}

void csBulletDynamicCollider::SetKinematic(bool kinematic)
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

void csBulletDynamicCollider::SetFriction(float friction)
{
  m_friction = friction;
  m_body->setFriction(friction);
}

void csBulletDynamicCollider::SetRestitution(float restitution)
{
  m_restitution = restitution;
  m_body->setRestitution(restitution);
}


void csBulletDynamicCollider::SetMass(float mass)
{
  m_mass = mass;

  UpdateInertia();
}

void csBulletDynamicCollider::SetInertia(const csVector3f &inertia)
{
  m_inertia = inertia;
  m_autoInertia = false;

  UpdateInertia();
}

void csBulletDynamicCollider::SetAutoInertia(bool autoInertia)
{
  m_autoInertia = autoInertia;

  UpdateInertia();
}

void csBulletDynamicCollider::UpdateInertia()
{
  if (m_autoInertia)
  {
    btCollisionShape *shape = m_shape ? m_shape : m_compoundShape;
    if (shape)
    {
      btVector3 btInertia;
      shape->calculateLocalInertia(m_mass, btInertia);
      m_inertia = csVector3f(btInertia.getX(), btInertia.getY(), btInertia.getZ());
    }
    else
    {
      m_inertia = csVector3f(0.0f, 0.0f, 0.0f);
    }
  }

  btVector3 inertia(m_inertia.x, m_inertia.y, m_inertia.z);
  m_body->setMassProps(m_mass, inertia);
  m_body->updateInertiaTensor();
}


void csBulletDynamicCollider::AttachToScene(csBulletScene *scene)
{
  m_scene = scene;
  if (m_scene && m_scene->GetBulletScene())
  {
    m_scene->GetBulletScene()->addRigidBody(m_body);
  }
}

void csBulletDynamicCollider::DetachFromScene(csBulletScene *scene)
{
  if (m_scene && m_scene->GetBulletScene())
  {

    DetachJoints(scene);
    m_scene->GetBulletScene()->removeRigidBody(m_body);
  }
  m_scene = 0;
}



void csBulletDynamicCollider::MotionState::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_parent->m_transformation.m00));
}

void csBulletDynamicCollider::MotionState::setWorldTransform(const btTransform& worldTrans)
{
  worldTrans.getOpenGLMatrix(static_cast<btScalar*>(&m_parent->m_transformation.m00));


  if (m_parent->m_scene && !m_parent->IsKinematic())
  {
    m_parent->m_scene->DynamicColliderChanged(m_parent);
  }
  // inform bullet system that this dynamic collider has changed
}

void csBulletDynamicCollider::PropagateTransformation()
{
  if (m_transformationCallback)
  {
    m_transformationCallback->TransformationChanged(m_transformation);
  }
}


void csBulletDynamicCollider::DetachJoints(csBulletScene *scene)
{
  for (size_t i = 0, in = m_joints.size(); i < in; ++i)
  {
    m_joints[i]->DetachFromScene(scene);
  }
}

void csBulletDynamicCollider::AddJoint(csBulletJoint *joint)
{
  m_joints.push_back(joint);
}


void csBulletDynamicCollider::RemoveJoint(csBulletJoint *joint)
{
  for (size_t i = 0, in = m_joints.size(); i < in; ++i)
  {
    if (m_joints[i] == joint)
    {
      m_joints.erase(m_joints.begin() + i);
    }
  }
}

