#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/csjointstate.hh>

#include <cobalt/entity/cshingejointstate.refl.hh>



namespace cs
{
struct iPhysicsHingeJoint;

CS_CLASS()
class CSE_API HingeJointState : public CS_SUPER(cs::JointState)
{
  CS_CLASS_GEN;
public:
  HingeJointState();
  virtual ~HingeJointState();

protected:
  virtual void UpdateTransformation();

  virtual void OnAssembled();
  virtual void OnAttachedToScene(cs::EntityScene * scene);
  virtual void OnDetachedFromScene(cs::EntityScene * scene);

private:
  cs::iPhysicsHingeJoint* m_hingeJoint;
};

}
