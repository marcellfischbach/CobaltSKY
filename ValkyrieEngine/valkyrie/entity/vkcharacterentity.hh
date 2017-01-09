#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/vkentity.hh>
#include <valkyrie/entity/vktransformation.hh>
#include <valkyrie/entity/vkcharacterentity.refl.hh>


struct iPhysicsCapsuleCharacterController;
class vkCharacterEntityTransformationCallback;
VK_CLASS()
class VKE_API vkCharacterEntity : public VK_SUPER(vkEntity)
{
  VK_CLASS_GEN;
public:
  vkCharacterEntity();
  virtual ~vkCharacterEntity();

  void Rotate(float angle);
  void SetRotation(float rotation);
  float GetRotation() const;

  virtual void FinishTransformation();
  void TransformationChanged(const vkMatrix4f &transformation);

  void SetRadius(float radius);
  float GetRadius() const;

  void SetHeight(float height);
  float GetHeight() const;

  void SetWalkDirection(const vkVector3f &walkDirection);

  void Jump();

protected:
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  vkCharacterEntityTransformationCallback *m_callback;
  iPhysicsCapsuleCharacterController *m_characterController;

  float m_radius;
  float m_height;
  float m_rotation;
};


VK_FORCEINLINE float vkCharacterEntity::GetRadius() const
{
  return m_radius;
}


VK_FORCEINLINE float vkCharacterEntity::GetHeight() const
{
  return m_height;
}

