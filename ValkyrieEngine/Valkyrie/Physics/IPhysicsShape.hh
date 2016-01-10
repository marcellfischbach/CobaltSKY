#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Physics/IPhysicsShape.refl.hh>

VK_INTERFACE()
struct VKE_API IPhysicsShape : public IObject
{
  VK_CLASS_GEN;
  virtual ~IPhysicsShape () { }

};