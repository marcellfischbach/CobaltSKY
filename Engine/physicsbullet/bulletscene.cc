

#include <physicsbullet/bulletscene.hh>
#include <physicsbullet/bulletcapsulecharactercontroller.hh>
#include <physicsbullet/bulletdynamiccollider.hh>
#include <physicsbullet/bulletjoint.hh>
#include <physicsbullet/bulletstaticcollider.hh>

csBulletScene::csBulletScene()
  : cs::iPhysicsScene()
{
  CS_CLASS_GEN_CONSTR;
}

csBulletScene::~csBulletScene()
{

}


bool csBulletScene::Initialize(bool softBody)
{
  btDefaultCollisionConstructionInfo constructionInfo;

  //
  // create the collision configuration. 
  if (softBody)
  {
    //m_config = new btSoftBody
  }
  else
  {
    m_config = new btDefaultCollisionConfiguration(constructionInfo);
  }

  if (!m_config)
  {
    return false;
  }
  m_dispatcher = new btCollisionDispatcher(m_config);

  // 
  // create the broadphase interface
  m_broadphaseInterface = 0;
  bool dbvt = true;
  if (dbvt)
  {
    m_broadphaseInterface = new btDbvtBroadphase();
  }

  if (!m_broadphaseInterface)
  {
    return false;
  }

  m_constraintSolver = new btSequentialImpulseConstraintSolver();

  if (softBody)
  {

  }
  else
  {
    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphaseInterface, m_constraintSolver, m_config);
  }

  if (!m_world)
  {
    return false;
  }

  m_world->setGravity(btVector3(0.0f, 0.0f, -9.81f));

  return true;
}

void csBulletScene::AddCollider(cs::iPhysicsCollider *collider)
{
  if (!collider)
  {
    return;
  }

  switch (collider->GetType())
  {
  case cs::ePCT_Static:
    AddStaticCollider(static_cast<cs::iPhysicsStaticCollider*>(collider));
    break;
  case cs::ePCT_Dynamic:
  case cs::ePCT_Kinematic:
    AddDynamicCollider(static_cast<cs::iPhysicsDynamicCollider*>(collider));
    break;
  }
}

void csBulletScene::RemoveCollider(cs::iPhysicsCollider *collider)
{
  if (!collider)
  {
    return;
  }

  switch (collider->GetType())
  {
  case cs::ePCT_Static:
    RemoveStaticCollider(static_cast<cs::iPhysicsStaticCollider*>(collider));
    break;
  case cs::ePCT_Dynamic:
  case cs::ePCT_Kinematic:
    RemoveDynamicCollider(static_cast<cs::iPhysicsDynamicCollider*>(collider));
    break;
  }
}


void csBulletScene::AddStaticCollider(cs::iPhysicsStaticCollider *staticCollider)
{
  if (!staticCollider)
  {
    return;
  }
  csBulletStaticCollider *btCollider = static_cast<csBulletStaticCollider*>(staticCollider);
  if (btCollider)
  {
    btCollider->AttachToScene(this);
  }
}

void csBulletScene::RemoveStaticCollider(cs::iPhysicsStaticCollider *staticCollider)
{
  if (!staticCollider)
  {
    return;
  }
  csBulletStaticCollider *btCollider = static_cast<csBulletStaticCollider*>(staticCollider);
  if (btCollider)
  {
    btCollider->DetachFromScene(this);
  }
}

void csBulletScene::AddDynamicCollider(cs::iPhysicsDynamicCollider *dynamicCollider)
{
  if (!dynamicCollider)
  {
    return;
  }
  csBulletDynamicCollider *btCollider = static_cast<csBulletDynamicCollider*>(dynamicCollider);
  if (btCollider)
  {
    btCollider->AttachToScene(this);
  }
}

void csBulletScene::RemoveDynamicCollider(cs::iPhysicsDynamicCollider *dynamicCollider)
{
  if (!dynamicCollider)
  {
    return;
  }
  csBulletDynamicCollider *btCollider = static_cast<csBulletDynamicCollider*>(dynamicCollider);
  if (btCollider)
  {
    btCollider->DetachFromScene(this);
  }
}


void csBulletScene::AddCharacterController(cs::iPhysicsCharacterController *controller)
{
  if (!controller)
  {
    return;
  }
  csBulletCapsuleCharacterController *capsController = cs::QueryClass<csBulletCapsuleCharacterController>(controller);
  if (capsController)
  {
    m_characterControllers.push_back(capsController);
    capsController->AttachToScene(this);
  }
}

void csBulletScene::RemoveCharacterController(cs::iPhysicsCharacterController *controller)
{
  if (!controller)
  {
    return;
  }
  csBulletCapsuleCharacterController *capsController = cs::QueryClass<csBulletCapsuleCharacterController>(controller);
  if (capsController)
  {
    for (size_t i = 0, in = m_characterControllers.size(); i < in; ++i)
    {
      if (m_characterControllers[i] == capsController)
      {
        m_characterControllers.erase(m_characterControllers.begin() + i);
        break;
      }
    }
    capsController->DetachFromScene(this);
  }
}


void csBulletScene::AddJoint(cs::iPhysicsJoint *joint)
{
  csBulletJoint *j = cs::QueryClass<csBulletJoint>(joint);
  if (j)
  {
    j->AttachToScene(this);
  }
}


void csBulletScene::RemoveJoint(cs::iPhysicsJoint *joint)
{
  csBulletJoint *j = cs::QueryClass<csBulletJoint>(joint);
  if (j)
  {
    j->DetachFromScene(this);
  }

}

void csBulletScene::BodyChanged(csBulletBody *body)
{
  m_changedBodies.Add(body);
}

void csBulletScene::DynamicColliderChanged(csBulletDynamicCollider *dynamicCollider)
{
  m_changedDynamicColliders.Add(dynamicCollider);
}

void csBulletScene::StepSimulation(float tpf)
{
  m_changedBodies.Clear();
  m_changedDynamicColliders.Clear();
  m_world->stepSimulation(tpf);
}

void csBulletScene::UpdateColliders()
{
  for (size_t i = 0; i < m_changedDynamicColliders.length; ++i)
  {
    m_changedDynamicColliders[i]->PropagateTransformation();
  }

  for (size_t i = 0, in= m_characterControllers.size(); i < in; ++i)
  {
    m_characterControllers[i]->UpdateCallbacks();
  }
}
