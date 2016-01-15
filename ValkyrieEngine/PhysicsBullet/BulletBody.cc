

#include <PhysicsBullet/BulletBody.hh>
#include <PhysicsBullet/BulletScene.hh>
#include <PhysicsBullet/BulletShape.hh>
#include <bullet/btBulletDynamicsCommon.h>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>

vkBulletBody::vkBulletBody()
  : IPhysicsBody()
  , m_bodyMode(ePBM_Static)
  , m_entity(0)
  , m_scene(0)
  , m_mass(0.0f)
  , m_inertia(0.0f, 0.0f, 0.0f)
  , m_autoInertia(false)
  , m_friction(0.5f)
  , m_restitution(0.5f)
  , m_bodiesDirty(false)
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
  if (m_mass != mass)
  {
    m_mass = mass;
    m_bodiesDirty = true;
  }
}

float vkBulletBody::GetMass() const
{
  return m_mass;
}

void vkBulletBody::SetFriction(float friction)
{
  if (m_friction != friction)
  {
    m_friction = friction;
    m_bodiesDirty = true;
  }
}

float vkBulletBody::GetFriction() const
{
  return m_friction;
}


void vkBulletBody::SetRestitution(float restitution)
{
  if (m_restitution != restitution)
  {
    m_restitution = restitution;
    m_bodiesDirty = true;
  }
}

float vkBulletBody::GetRestitution() const
{
  return m_restitution;
}

void vkBulletBody::SetInertia(const vkVector3f &inertia)
{
  m_inertia = inertia;
  m_autoInertia = false;
  m_bodiesDirty = true;
}

const vkVector3f &vkBulletBody::GetInertia() const
{
  return m_inertia;
}

void vkBulletBody::SetAutoInertia(bool autoInertia)
{
  if (m_autoInertia != autoInertia)
  {
    m_autoInertia = autoInertia;
    m_bodiesDirty = true;
  }
}

void vkBulletBody::Update()
{
  if (!m_bodiesDirty)
  {
    return;
  }

  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    m_bodies[i]->DetachFromScene(m_scene);
    delete m_bodies[i];
  }
  m_bodies.clear();

  CreateBodies();
}

void vkBulletBody::CreateBodies()
{
  if (m_bodyMode == ePBM_Dynamic)
  {
    vkBulletBodyImpl *impl = new vkBulletBodyImpl(this);
    impl->AttachShapes(m_shapes);
    impl->CreateRigidBody();
    m_bodies.push_back(impl);
  }
  else
  {
    for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
    {
      vkBulletBodyImpl *impl = new vkBulletBodyImpl(this);
      impl->AttachShape(m_shapes[i]);
      impl->CreateRigidBody();
      m_bodies.push_back(impl);
    }
  }



  FinishTransformation();
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


void vkBulletBody::AttachShape(IPhysicsShape *shape)
{
  if (shape)
  {
    shape->AddRef();
    m_shapes.push_back(static_cast<vkBulletShape*>(shape));
    m_bodiesDirty = true;
  }
}


void vkBulletBody::DetachShape(IPhysicsShape *shape)
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

  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    if (m_shapes[i] == btShape)
    {
      m_shapes.erase(m_shapes.begin() + i);
      break;
    }
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

void vkBulletBody::AttachShape(vkPhysicsShapeContainer *shapes)
{
  for (vkSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    AttachShape(shapes->GetShape(i));
  }
}

void vkBulletBody::DetachShape(vkPhysicsShapeContainer *shapes)
{
  for (vkSize i = 0, in = shapes->GetNumberOfShapes(); i < in; ++i)
  {
    DetachShape(shapes->GetShape(i));
  }
}

void vkBulletBody::DynamicallyChanged(const vkMatrix4f &transform)
{
  if (m_bodyMode == ePBM_Dynamic)
  {
    if (m_entity && m_scene)
    {
      m_transform.Set(transform);
      m_scene->BodyChanged(this);
    }
  }
}



void vkBulletBody::AttachToScene(vkBulletScene *scene)
{
  m_scene = scene;
  if (m_bodies.size() == 0)
  {
    CreateBodies();
  }

  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    m_bodies[i]->AttachToScene(scene);
  }
}

void vkBulletBody::DetachFromScene(vkBulletScene *scene)
{
  m_scene = 0;
  for (size_t i = 0, in = m_bodies.size(); i < in; ++i)
  {
    m_bodies[i]->DetachFromScene(scene);
  }
}




vkBulletBodyImpl::vkBulletBodyImpl(vkBulletBody *body)
  : m_collisionShape(0)
  , m_compoundShape(0)
  , m_rigidBody(0)
  , m_collisionObject(0)
  , m_bulletBody(body)

{

}

vkBulletBodyImpl::~vkBulletBodyImpl()
{
  if (m_rigidBody)
  {
    delete m_rigidBody;
    m_rigidBody = 0;
    m_collisionObject = 0;
  }
  if (m_collisionObject)
  {
    delete m_collisionObject;
    m_collisionObject = 0;
  }

  m_collisionShape = 0;
  m_compoundShape = 0;
  m_bulletBody = 0;

}

void vkBulletBodyImpl::AttachShapes(std::vector<vkBulletShape*> &shapes)
{
  if (shapes.size() == 0)
  {
    return;
  }
  else if (shapes.size() == 1)
  {
    AttachShape(shapes[0]);
  }
  else
  {
    m_compoundShape = new btCompoundShape();

    for (size_t i = 0, in = shapes.size(); i < in; ++i)
    {
      vkBulletShape *shape = shapes[i];

      btTransform trans;
      trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&shape->GetLocalTransform().m00));
      m_compoundShape->addChildShape(trans, shape->GetBulletShape());

      m_shapes.push_back(shape);
      shape->AddRef();
    }
  }
}

void vkBulletBodyImpl::AttachShape(vkBulletShape *shape)
{
  if (!shape)
  {
    return;
  }

  if (m_collisionShape)
  {
    // this bullet body already has an attached shape
    return;
  }

  m_collisionShape = shape->GetBulletShape();
  m_shapes.push_back(shape);
  shape->AddRef();
}


bool vkBulletBodyImpl::DetachShape(vkBulletShape *shape)
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    vkBulletShape *myShape = m_shapes[i];
    if (myShape == shape)
    {
      if (m_collisionShape)
      {
        m_collisionShape = 0;
      }
      else if (m_compoundShape)
      {
        // TODO: remove the shape
      }
      shape->Release();
      m_shapes.erase(m_shapes.begin() + i);
      return true;
    }
  }
  return false;
}

bool vkBulletBodyImpl::HasShape() const
{
  return m_shapes.size() > 0;
}


void vkBulletBodyImpl::CreateRigidBody()
{
  btCollisionShape *shape = m_collisionShape ? m_collisionShape : m_compoundShape;
  if (!shape)
  {
    return;
  }


  float mass = m_bulletBody->GetMass();
  btVector3 inertia(m_bulletBody->m_inertia.x, m_bulletBody->m_inertia.y, m_bulletBody->m_inertia.z);
  if (m_bulletBody->m_autoInertia)
  {
    shape->calculateLocalInertia(mass, inertia);
  }


  m_rigidBody = 0;
  m_collisionObject = 0;

  switch (m_bulletBody->m_bodyMode)
  {
  case ePBM_Static:
    m_collisionObject = new btCollisionObject();
    m_collisionObject->setCollisionShape(shape);
    UpdateTransform(m_bulletBody->GetMatrix());
    break;

  case ePBM_Kinematic:
  case ePBM_Dynamic:
    {
      btRigidBody::btRigidBodyConstructionInfo info(mass, 0, shape, inertia);
      m_rigidBody = new btRigidBody(info);
      m_rigidBody->updateInertiaTensor();
      if (m_bulletBody->m_bodyMode == ePBM_Kinematic)
      {
        m_rigidBody->setFlags(m_rigidBody->getFlags() & btRigidBody::CF_KINEMATIC_OBJECT);
      }
      m_collisionObject = m_rigidBody;
    }
    break;
  }

  m_collisionObject->setFriction(m_bulletBody->m_friction);
  m_collisionObject->setRestitution(m_bulletBody->m_restitution);
}

void vkBulletBodyImpl::UpdateTransform(const vkMatrix4f &transform)
{
  if (!m_collisionObject)
  {
    return;
  }

  if (m_shapes.size() == 0)
  {
    return;
  }

  vkMatrix4f absTrans;
  if (m_shapes.size() == 1)
  {
    // if we have only one shape we don't have a compound collision object
    // so we must handle the local transformation of the shape by ourself.
    vkMatrix4f::Mult(transform, m_shapes[0]->GetLocalTransform(), absTrans);
  }
  else
  {
    // if we have more than one shape we have a compound shape
    // so the local transformation of the shape is already contained within
    // the compound collision shape
    absTrans = transform;
  }


  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&absTrans.m00));
  m_collisionObject->setWorldTransform(trans);

}

void vkBulletBodyImpl::getWorldTransform(btTransform& worldTrans) const
{
  worldTrans.setFromOpenGLMatrix(static_cast<const btScalar*>(&m_bulletBody->GetMatrix().m00));
}

void vkBulletBodyImpl::setWorldTransform(const btTransform &worldTrans)
{
  vkMatrix4f trans;
  worldTrans.getOpenGLMatrix(static_cast<btScalar*>(&trans.m00));

  m_bulletBody->DynamicallyChanged(trans);
}



void vkBulletBodyImpl::AttachToScene(vkBulletScene *scene)
{
  if (scene)
  {
    if (m_rigidBody)
    {
      m_rigidBody->setMotionState(this);
      scene->GetBulletScene()->addRigidBody(m_rigidBody);
    }
    else if (m_collisionObject)
    {
      scene->GetBulletScene()->addCollisionObject(m_collisionObject);
    }
  }

}

void vkBulletBodyImpl::DetachFromScene(vkBulletScene *scene)
{
  if (scene)
  {
    if (m_rigidBody)
    {
      scene->GetBulletScene()->removeRigidBody(m_rigidBody);
      m_rigidBody->setMotionState(0);
    }
    else
    {
      scene->GetBulletScene()->removeCollisionObject(m_collisionObject);
    }
  }
}

