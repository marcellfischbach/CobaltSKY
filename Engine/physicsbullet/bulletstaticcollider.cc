
#include <physicsbullet/bulletstaticcollider.hh>
#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletshape.hh>
#include <cobalt/physics/iphysicsshape.hh>
#include <cobalt/physics/csphysicsshape.hh>


cs::BulletStaticCollider::BulletStaticCollider()
  : cs::iPhysicsStaticCollider()
  , m_friction(0.0f)
  , m_restitution(0.0f)
{
}

cs::BulletStaticCollider::~BulletStaticCollider()
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

void cs::BulletStaticCollider::AttachShape(cs::iPhysicsShape *shape)
{
  if (!shape)
  {
    return;
  }

  shape->AddRef();

  cs::BulletShape *btShape = static_cast<cs::BulletShape*>(shape);

  Data data;
  data.btShape = btShape->GetBulletShape();
  data.localTransform = shape->GetLocalTransform();
  data.shape = btShape;
  data.object = new btCollisionObject();
  data.object->setCollisionShape(data.btShape);
  data.object->setFriction(m_friction);
  data.object->setRestitution(m_restitution);

  cs::Matrix4f global;
  cs::Matrix4f::Mult(m_transformation, data.localTransform, global);
  // global.Debug("Global");
  btTransform trans;
  trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&global.m00));
  data.object->setWorldTransform(trans);

  m_shapes.push_back(data);
}

void cs::BulletStaticCollider::DetachShape(cs::iPhysicsShape *shape)
{
  // not implemented yet
}

void cs::BulletStaticCollider::AttachShape(cs::PhysicsShape *shapes)
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

void cs::BulletStaticCollider::DetachShape(cs::PhysicsShape *shapes)
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

cs::Transformation cs::BulletStaticCollider::GetTransform()
{
  return cs::Transformation(&m_transformation, 0, 0, 0);
}


void cs::BulletStaticCollider::FinishTransformation()
{
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    cs::Matrix4f global;
    cs::Matrix4f::Mult(m_transformation, data.localTransform, global);

    // global.Debug("Final global");
    btTransform trans;
    trans.setFromOpenGLMatrix(static_cast<const btScalar*>(&global.m00));
    data.object->setWorldTransform(trans);
  }
}



void cs::BulletStaticCollider::SetFriction(float friction)
{
  m_friction = friction;
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    data.object->setFriction(friction);
  }
}

void cs::BulletStaticCollider::SetRestitution(float restitution)
{
  m_restitution = restitution;
  for (size_t i = 0, in = m_shapes.size(); i < in; ++i)
  {
    Data &data = m_shapes[i];
    data.object->setRestitution(restitution);
  }
}



void cs::BulletStaticCollider::AttachToScene(cs::BulletScene *scene)
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

void cs::BulletStaticCollider::DetachFromScene(cs::BulletScene *scene)
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
