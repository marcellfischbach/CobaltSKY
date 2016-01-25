
#include <Valkyrie/Entity/Character.hh>
#include <Valkyrie/Entity/Scene.hh>
#include <Valkyrie/Entity/SpatialState.hh>
#include <Valkyrie/Entity/Transformation.hh>
#include <Valkyrie/Engine.hh>

#include <Valkyrie/Physics/IPhysicsCharacterController.hh>
#include <Valkyrie/Physics/IPhysicsScene.hh>

class vkCharacterEntityTransformationCallback : public ITransformationCallback
{
public:
  vkCharacterEntityTransformationCallback(vkCharacterEntity *entity)
    : m_entity(entity)
  {

  }

  void TransformationChanged(const vkMatrix4f &transformation)
  {
    m_entity->TransformationChanged(transformation);
  }

  vkCharacterEntity *m_entity;
};

vkCharacterEntity::vkCharacterEntity()
  : vkEntity()
  , m_height(1.8)
  , m_radius(0.35)
{
  m_callback = new vkCharacterEntityTransformationCallback(this);
  m_characterController = vkEngine::Get()->GetPhysicsSystem()->CreateCapsulseCharacterController();
  m_characterController->Initialize(m_height, m_radius);
  m_characterController->SetTransformationCallback(m_callback);
}

vkCharacterEntity::~vkCharacterEntity()
{
  m_characterController->SetTransformationCallback(0);
  delete m_callback;

}

void vkCharacterEntity::SetHeight(float height)
{
  m_height = height;
}

void vkCharacterEntity::SetRadius(float radius)
{
  m_radius = radius;
}

void vkCharacterEntity::UpdateScene(vkEntityScene *scene)
{
  vkEntityScene *oldScene = GetScene();
  if (oldScene)
  {
    oldScene->GetPhysicsScene()->RemoveCharacterController(m_characterController);
  }
  
  if (scene)
  {
    scene->GetPhysicsScene()->AddCharacterController(m_characterController);
  }
}

void vkCharacterEntity::FinishTransformation()
{
  vkEntity::FinishTransformation();

  m_characterController->Warp(GetRootState()->GetGlobalTransformation());
}

void vkCharacterEntity::TransformationChanged(const vkMatrix4f &transformation)
{
  GetTransformation().SetTransformation(transformation);
  PerformTransformation();
}


void vkCharacterEntity::SetWalkDirection(const vkVector3f &walkDirection)
{
  m_characterController->SetGlobalWalkDirection(walkDirection);
}

void vkCharacterEntity::Jump()
{
  m_characterController->Jump();
}