#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Physics/IPhysicsBody.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsBody : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsBody () { }

};