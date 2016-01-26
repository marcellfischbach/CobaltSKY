

#include <PhysicsBullet/BulletScene.hh>
#include <PhysicsBullet/BulletCharacterController.hh>
#include <PhysicsBullet/BulletDynamicCollider.hh>
#include <PhysicsBullet/BulletJoints.hh>
#include <PhysicsBullet/BulletStaticCollider.hh>

vkBulletScene::vkBulletScene()
  : IPhysicsScene()
{
  VK_CLASS_GEN_CONSTR;
}

vkBulletScene::~vkBulletScene()
{

}


bool vkBulletScene::Initialize(bool softBody)
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


void vkBulletScene::AddDynamicCollider(IPhysicsDynamicCollider *dynamicCollider)
{
  vkBulletDynamicCollider *btCollider = static_cast<vkBulletDynamicCollider*>(dynamicCollider);
  if (btCollider)
  {
    btCollider->AttachToScene(this);
  }
}

void vkBulletScene::RemoveDynamicCollider(IPhysicsDynamicCollider *dynamicCollider)
{
  vkBulletDynamicCollider *btCollider = static_cast<vkBulletDynamicCollider*>(dynamicCollider);
  if (btCollider)
  {
    btCollider->DetachFromScene(this);
  }
}

void vkBulletScene::AddStaticCollider(IPhysicsStaticCollider *staticCollider)
{
  vkBulletStaticCollider *btCollider = static_cast<vkBulletStaticCollider*>(staticCollider);
  if (btCollider)
  {
    btCollider->AttachToScene(this);
  }
}

void vkBulletScene::RemoveStaticCollider(IPhysicsStaticCollider *staticCollider)
{
  vkBulletStaticCollider *btCollider = static_cast<vkBulletStaticCollider*>(staticCollider);
  if (btCollider)
  {
    btCollider->DetachFromScene(this);
  }
}

void vkBulletScene::AddCharacterController(IPhysicsCharacterController *controller)
{
  vkBulletCapsuleCharacterController *capsController = vkQueryClass<vkBulletCapsuleCharacterController>(controller);
  if (capsController)
  {
    m_characterControllers.push_back(capsController);
    capsController->AttachToScene(this);
  }
}

void vkBulletScene::RemoveCharacterController(IPhysicsCharacterController *controller)
{
  vkBulletCapsuleCharacterController *capsController = vkQueryClass<vkBulletCapsuleCharacterController>(controller);
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


void vkBulletScene::AddJoint(IPhysicsJoint *joint)
{
  vkBulletJoint *j = vkQueryClass<vkBulletJoint>(joint);
  if (j)
  {
    j->AttachToScene(this);
  }
}


void vkBulletScene::RemoveJoint(IPhysicsJoint *joint)
{
  vkBulletJoint *j = vkQueryClass<vkBulletJoint>(joint);
  if (j)
  {
    j->DetachFromScene(this);
  }

}

void vkBulletScene::BodyChanged(vkBulletBody *body)
{
  m_changedBodies.Add(body);
}

void vkBulletScene::DynamicColliderChanged(vkBulletDynamicCollider *dynamicCollider)
{
  m_changedDynamicColliders.Add(dynamicCollider);
}

void vkBulletScene::StepSimulation(float tpf)
{
  m_changedBodies.Clear();
  m_changedDynamicColliders.Clear();
  m_world->stepSimulation(tpf);
}

void vkBulletScene::UpdateColliders()
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