#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/Entity.hh>
#include <Valkyrie/Entity/Character.refl.hh>
#include <Valkyrie/Entity/ITransformationCallback.hh>


struct IPhysicsCapsuleCharacterController;
class vkCharacterEntityTransformationCallback;
VK_CLASS()
class VKE_API vkCharacterEntity : public vkEntity
{
  VK_CLASS_GEN;
public:
  vkCharacterEntity();
  virtual ~vkCharacterEntity();


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
  IPhysicsCapsuleCharacterController *m_characterController;

  float m_radius;
  float m_height;
};


VK_FORCEINLINE float vkCharacterEntity::GetRadius() const
{
  return m_radius;
}


VK_FORCEINLINE float vkCharacterEntity::GetHeight() const
{
  return m_height;
}

