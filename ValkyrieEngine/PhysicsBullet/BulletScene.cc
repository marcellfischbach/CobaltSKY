

#include <PhysicsBullet/BulletScene.hh>
#include <PhysicsBullet/BulletBody.hh>

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


void vkBulletScene::AddBody(IPhysicsBody *body)
{
  vkBulletBody *btBody = static_cast<vkBulletBody*>(body);
  if (btBody)
  {
    btBody->AttachToScene(this);
  }
}

void vkBulletScene::RemoveBody(IPhysicsBody *body)
{
  vkBulletBody *btBody = static_cast<vkBulletBody*>(body);
  if (btBody)
  {
    btBody->DetachFromScene(this);
  }
}


void vkBulletScene::BodyChanged(vkBulletBody *body)
{
  m_changedBodies.Add(body);
}


void vkBulletScene::StepSimulation(float tpf)
{
  m_changedBodies.Clear();
  m_world->stepSimulation(tpf);
}

void vkBulletScene::UpdateEntityTransformation()
{
  for (size_t i = 0; i < m_changedBodies.length; ++i)
  {
    m_changedBodies[i]->GetEntity()->UpdatePhysicsTransformation();
  }
}

