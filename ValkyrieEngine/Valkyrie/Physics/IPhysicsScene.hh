#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Physics/IPhysicsScene.refl.hh>

struct IPhysicsBody;

VK_INTERFACE()
struct VKE_API IPhysicsScene : public IObject
{
  VK_CLASS_GEN;
public:
  virtual ~IPhysicsScene () { }

  virtual void AddBody(IPhysicsBody *body) = 0;
  virtual void RemoveBody(IPhysicsBody *body) = 0;

  //virtual void AddCollider(IPhysicsCollider *collider) = 0;


  virtual void StepSimulation(float tpf = 1.0f / 60.0f) = 0;
  virtual void UpdateEntityTransformation() = 0;

};