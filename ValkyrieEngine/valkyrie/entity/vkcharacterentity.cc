
#include <valkyrie/entity/vkcharacterentity.hh>
#include <valkyrie/entity/itransformationcallback.hh>
#include <valkyrie/entity/vkentityscene.hh>
#include <valkyrie/entity/vkspatialstate.hh>
#include <valkyrie/entity/vktransformation.hh>
#include <valkyrie/vkengine.hh>

#include <valkyrie/physics/iphysicscapsulecharactercontroller.hh>
#include <valkyrie/physics/iphysicsscene.hh>

class vkCharacterEntityTransformationCallback : public ITransformationCallback
{
public:
  vkCharacterEntityTransformationCallback(vkCharacterEntity *entity)
    : m_entity(entity)
  {

  }

  void TransformationChanged(const vkMatrix4f &transformation)
  {
    m_entity->GetTransformation().GetTransformation(m_transform);
    transformation.GetTranslation(m_translation);
    m_transform.SetTranslation(m_translation);
    m_entity->TransformationChanged(m_transform);
  }

  vkVector3f m_translation;
  vkMatrix4f m_transform;
  vkCharacterEntity *m_entity;
};

vkCharacterEntity::vkCharacterEntity()
  : vkEntity()
  , m_height(1.8f)
  , m_radius(0.35f)
  , m_rotation(0.0f)
{
  m_callback = new vkCharacterEntityTransformationCallback(this);
  m_characterController = vkEng->CreateCapsulseCharacterController();
  m_characterController->Initialize(m_height, m_radius);
  m_characterController->SetTransformationCallback(m_callback);

}

vkCharacterEntity::~vkCharacterEntity()
{
  m_characterController->SetTransformationCallback(0);
  delete m_callback;

}

void vkCharacterEntity::Rotate(float angle)
{
  m_rotation += angle;
  GetTransformation().SetRotationZ(m_rotation);
}

void vkCharacterEntity::SetRotation(float rotation)
{
  m_rotation = rotation;
  GetTransformation().SetRotationZ(m_rotation);
}

float vkCharacterEntity::GetRotation() const
{
  return m_rotation;
}

void vkCharacterEntity::SetHeight(float height)
{
  m_height = height;
}

void vkCharacterEntity::SetRadius(float radius)
{
  m_radius = radius;
}

void vkCharacterEntity::OnAttachedToScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddCharacterController(m_characterController);
  }
}

void vkCharacterEntity::OnDetachedFromScene(vkEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveCharacterController(m_characterController);
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
