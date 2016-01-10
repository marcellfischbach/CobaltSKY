#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Physics/IPhysicsSystem.refl.hh>

struct IPhysicsBody;
struct IPhysicsScene;
struct IPhysicsShape;

VK_INTERFACE()
struct VKE_API IPhysicsSystem : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsSystem () { }

  virtual void Initialize() = 0;



  virtual IPhysicsScene *CreateScene() = 0;

};