#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/vkjointstate.hh>

#include <valkyrie/entity/vkhingejointstate.refl.hh>


struct IPhysicsHingeJoint;

VK_CLASS()
class VKE_API vkHingeJointState : public VK_SUPER(vkJointState)
{
  VK_CLASS_GEN;
public:
  vkHingeJointState();
  virtual ~vkHingeJointState();

protected:
  virtual void UpdateTransformation();

  virtual void OnAssembled();
  virtual void OnAttachedToScene(vkEntityScene *scene);
  virtual void OnDetachedFromScene(vkEntityScene *scene);

private:
  IPhysicsHingeJoint *m_hingeJoint;
};
