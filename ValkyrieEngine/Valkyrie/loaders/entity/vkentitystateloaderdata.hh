#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/loaders/entity/vkentitystateloaderdata.refl.hh>

class vkEntity;
class vkEntityState;

VK_CLASS()
class vkEntityStateLoaderData : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
  vkEntityStateLoaderData() : vkObject() { }
  virtual ~vkEntityStateLoaderData() { }

public:
  vkEntityState *state;
  vkEntity *entity;
};

