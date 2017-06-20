
#include <cobalt/entity/cscharacterentity.hh>
#include <cobalt/entity/itransformationcallback.hh>
#include <cobalt/entity/csentityscene.hh>
#include <cobalt/entity/csspatialstate.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/csengine.hh>

#include <cobalt/physics/iphysicscapsulecharactercontroller.hh>
#include <cobalt/physics/iphysicsscene.hh>

class csCharacterEntityTransformationCallback : public iTransformationCallback
{
public:
  csCharacterEntityTransformationCallback(csCharacterEntity *entity)
    : m_entity(entity)
  {

  }

  void TransformationChanged(const csMatrix4f &transformation)
  {
    m_entity->GetTransformation().GetTransformation(m_transform);
    transformation.GetTranslation(m_translation);
    m_transform.SetTranslation(m_translation);
    m_entity->TransformationChanged(m_transform);
  }

  csVector3f m_translation;
  csMatrix4f m_transform;
  csCharacterEntity *m_entity;
};

csCharacterEntity::csCharacterEntity()
  : csEntity()
  , m_height(1.8f)
  , m_radius(0.35f)
  , m_rotation(0.0f)
{
  m_callback = new csCharacterEntityTransformationCallback(this);
  m_characterController = csEng->CreateCapsulseCharacterController();
  m_characterController->Initialize(m_height, m_radius);
  m_characterController->SetTransformationCallback(m_callback);

}

csCharacterEntity::~csCharacterEntity()
{
  m_characterController->SetTransformationCallback(0);
  delete m_callback;

}

void csCharacterEntity::Rotate(float angle)
{
  m_rotation += angle;
  GetTransformation().SetRotationZ(m_rotation);
}

void csCharacterEntity::SetRotation(float rotation)
{
  m_rotation = rotation;
  GetTransformation().SetRotationZ(m_rotation);
}

float csCharacterEntity::GetRotation() const
{
  return m_rotation;
}

void csCharacterEntity::SetHeight(float height)
{
  m_height = height;
}

void csCharacterEntity::SetRadius(float radius)
{
  m_radius = radius;
}

void csCharacterEntity::OnAttachedToScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->AddCharacterController(m_characterController);
  }
}

void csCharacterEntity::OnDetachedFromScene(csEntityScene *scene)
{
  if (scene)
  {
    scene->GetPhysicsScene()->RemoveCharacterController(m_characterController);
  }
  
}

void csCharacterEntity::FinishTransformation()
{
  csEntity::FinishTransformation();

  m_characterController->Warp(GetRootState()->GetGlobalTransformation());
}

void csCharacterEntity::TransformationChanged(const csMatrix4f &transformation)
{
  GetTransformation().SetTransformation(transformation);
  PerformTransformation();
}


void csCharacterEntity::SetWalkDirection(const csVector3f &walkDirection)
{
  m_characterController->SetGlobalWalkDirection(walkDirection);
}

void csCharacterEntity::Jump()
{
  m_characterController->Jump();
}
