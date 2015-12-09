#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Entity/EntityState.refl.hh>

VK_CLASS()
class VKE_API vkEntityState : public vkObject
{
  VK_CLASS_GEN;

public:
  virtual ~vkEntityState();

protected:
  vkEntityState();
};