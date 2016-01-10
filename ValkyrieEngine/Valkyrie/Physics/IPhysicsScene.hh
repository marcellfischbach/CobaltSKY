#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Physics/IPhysicsScene.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsScene : public IObject
{
  VK_CLASS_GEN;
public:
  virtual ~IPhysicsScene () { }



};