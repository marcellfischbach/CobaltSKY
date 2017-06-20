#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/entity/csentity.hh>
#include <cobalt/entity/cstransformation.hh>
#include <cobalt/entity/cscharacterentity.refl.hh>


struct iPhysicsCapsuleCharacterController;
class csCharacterEntityTransformationCallback;
CS_CLASS()
class CSE_API csCharacterEntity : public CS_SUPER(csEntity)
{
  CS_CLASS_GEN;
public:
  csCharacterEntity();
  virtual ~csCharacterEntity();

  void Rotate(float angle);
  void SetRotation(float rotation);
  float GetRotation() const;

  virtual void FinishTransformation();
  void TransformationChanged(const csMatrix4f &transformation);

  void SetRadius(float radius);
  float GetRadius() const;

  void SetHeight(float height);
  float GetHeight() const;

  void SetWalkDirection(const csVector3f &walkDirection);

  void Jump();

protected:
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

private:
  csCharacterEntityTransformationCallback *m_callback;
  iPhysicsCapsuleCharacterController *m_characterController;

  float m_radius;
  float m_height;
  float m_rotation;
};


CS_FORCEINLINE float csCharacterEntity::GetRadius() const
{
  return m_radius;
}


CS_FORCEINLINE float csCharacterEntity::GetHeight() const
{
  return m_height;
}

