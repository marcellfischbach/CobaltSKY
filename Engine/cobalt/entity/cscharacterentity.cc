
#include <cobalt/entity/cscharacterentity.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/csengine.hh>

#include <cobalt/physics/iphysicscapsulecharactercontroller.hh>
#include <cobalt/physics/iphysicsscene.hh>

namespace cs
{
class CharacterEntityTransformationCallback : public cs::iTransformationCallback
{
public:
  CharacterEntityTransformationCallback(cs::CharacterEntity* entity)
    : m_entity(entity)
  {

  }

  void TransformationChanged(const cs::Matrix4f& transformation)
  {
    m_entity->GetTransformation().GetTransformation(m_transform);
    transformation.GetTranslation(m_translation);
    m_transform.SetTranslation(m_translation);
    m_entity->TransformationChanged(m_transform);
  }

  cs::Vector3f m_translation;
  cs::Matrix4f m_transform;
  cs::CharacterEntity* m_entity;
};
}
cs::CharacterEntity::CharacterEntity()
  : cs::Entity()
  , m_height(1.8f)
  , m_radius(0.35f)
  , m_rotation(0.0f)
{
  m_callback = new cs::CharacterEntityTransformationCallback(this);
  m_characterController = csEng->CreateCapsulseCharacterController();
  m_characterController->Initialize(m_height, m_radius);
  m_characterController->SetTransformationCallback(m_callback);

}

cs::CharacterEntity::~CharacterEntity()
{
  m_characterController->SetTransformationCallback(0);
  delete m_callback;

}

void cs::CharacterEntity::Rotate(float angle)
{
  m_rotation += angle;
  GetTransformation().SetRotationZ(m_rotation);
}

void cs::CharacterEntity::SetRotation(float rotation)
{
  m_rotation = rotation;
  GetTransformation().SetRotationZ(m_rotation);
}

float cs::CharacterEntity::GetRotation() const
{
  return m_rotation;
}

void cs::CharacterEntity::SetHeight(float height)
{
  m_height = height;
}

void cs::CharacterEntity::SetRadius(float radius)
{
  m_radius = radius;
}

void cs::CharacterEntity::OnAttachedToScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddCharacterController(m_characterController);
  }
}

void cs::CharacterEntity::OnDetachedFromScene(cs::EntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveCharacterController(m_characterController);
  }
  
}

void cs::CharacterEntity::FinishTransformation()
{
  cs::Entity::FinishTransformation();

  m_characterController->Warp(GetRootState()->GetGlobalTransformation());
}

void cs::CharacterEntity::TransformationChanged(const cs::Matrix4f &transformation)
{
  GetTransformation().SetTransformation(transformation);
  PerformTransformation();
}


void cs::CharacterEntity::SetWalkDirection(const cs::Vector3f &walkDirection)
{
  m_characterController->SetGlobalWalkDirection(walkDirection);
}

void cs::CharacterEntity::Jump()
{
  m_characterController->Jump();
}
