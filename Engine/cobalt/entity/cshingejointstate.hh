#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/csjointstate.hh>

#include <cobalt/entity/cshingejointstate.refl.hh>


struct iPhysicsHingeJoint;

CS_CLASS()
class CSE_API csHingeJointState : public CS_SUPER(csJointState)
{
  CS_CLASS_GEN;
public:
  csHingeJointState();
  virtual ~csHingeJointState();

protected:
  virtual void UpdateTransformation();

  virtual void OnAssembled();
  virtual void OnAttachedToScene(csEntityScene *scene);
  virtual void OnDetachedFromScene(csEntityScene *scene);

private:
  iPhysicsHingeJoint *m_hingeJoint;
};
