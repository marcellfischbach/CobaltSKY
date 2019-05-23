#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/entity/cscharacterentity.refl.hh>



namespace cs
{
struct iPhysicsCapsuleCharacterController;
class CharacterEntityTransformationCallback;

CS_CLASS()
class CSE_API CharacterEntity : public CS_SUPER(cs::Entity)
{
  CS_CLASS_GEN;
public:
  CharacterEntity();
  virtual ~CharacterEntity();

  void Rotate(float angle);
  void SetRotation(float rotation);
  float GetRotation() const;

  virtual void FinishTransformation();
  void TransformationChanged(const cs::Matrix4f & transformation);

  void SetRadius(float radius);
  float GetRadius() const;

  void SetHeight(float height);
  float GetHeight() const;

  void SetWalkDirection(const cs::Vector3f & walkDirection);

  void Jump();

protected:
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

private:
  cs::CharacterEntityTransformationCallback* m_callback;
  cs::iPhysicsCapsuleCharacterController* m_characterController;

  float m_radius;
  float m_height;
  float m_rotation;
};

}
CS_FORCEINLINE float cs::CharacterEntity::GetRadius() const
{
  return m_radius;
}


CS_FORCEINLINE float cs::CharacterEntity::GetHeight() const
{
  return m_height;
}


