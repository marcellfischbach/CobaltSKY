
#include <PhysicsBullet/BulletStaticCollider.hh>
#include <PhysicsBullet/BulletScene.hh>
#include <PhysicsBullet/BulletShape.hh>
#include <Valkyrie/Physics/IPhysicsShape.hh>
#include <Valkyrie/Physics/PhysicsShapeContainer.hh>


vkBulletStaticCollider::vkBulletStaticCollider()
  : IPhysicsStaticCollider()
  , m_friction(0.0f)
  , m_restitution(0.0f)
{
}

vkBulletStaticCollider::~vkBulletStaticCollider()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    if (data.object)
    {
      delete data.object;
    }
    data.btShape = 0;
    if (data.shape)
    {
      data.shape->Release();
    }
  }
  m_shapes.clear();
}

void vkBulletStaticCollider::AttachShape(IPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  shape->AddRef();

  vkBulletShape *btShape = static_cast<vkBulletShape*>(shape);

  Data data;
  data.btShape = btShape->GetBulletShape();
  data.localTransform = shape->GetLocalTransform();
  data.shape = btShape;
  data.object = new btCollisionObject();
  data.object->setCollisionShape(data.btShape);
  data.object->setFriction(m_friction);
  data.object->setRestitution(m_restitution);

  vkMatrix4f global;
  vkMatrix4f::Mult(m_transformation, data.localTransform, global);
  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&global.m00));
  data.object->setWorldTransform(trans);

  m_shapes.push_back(data);
}

void vkBulletStaticCollider::DetachShape(IPhysicsShape *shape)
{
  // not implemented yet
}

void vkBulletStaticCollider::AttachShape(vkPhysicsShapeContainer *shapes)
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

void vkBulletStaticCollider::DetachShape(vkPhysicsShapeContainer *shapes)
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

vkTransformation vkBulletStaticCollider::GetTransform()
{
  return vkTransformation(&m_transformation, 0, 0, 0);
}


void vkBulletStaticCollider::FinishTransformation()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    vkMatrix4f global;
    vkMatrix4f::Mult(m_transformation, data.localTransform, global);
    btTransform trans;
    trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&global.m00));
    data.object->setWorldTransform(trans);
  }
}



void vkBulletStaticCollider::SetFriction(float friction)
{
  m_friction = friction;
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    data.object->setFriction(friction);
  }
}

void vkBulletStaticCollider::SetRestitution(float restitution)
{
  m_restitution = restitution;
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    data.object->setRestitution(restitution);
  }
}



void vkBulletStaticCollider::AttachToScene(vkBulletScene *scene)
{
  m_scene = scene;
  if (m_scene && m_scene->GetBulletScene())
  {
    btDiscreteDynamicsWorld *world = m_scene->GetBulletScene();
    for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
    {
      Data &data = m_shapes[i];
      world->addCollisionObject(data.object);
    }
  }
}

void vkBulletStaticCollider::DetachFromScene(vkBulletScene *scene)
{
  if (m_scene && m_scene->GetBulletScene())
  {
    btDiscreteDynamicsWorld *world = m_scene->GetBulletScene();
    for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
    {
      Data &data = m_shapes[i];
      world->removeCollisionObject(data.object);
    }
  }
  m_scene = 0;
}
