#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/loaders/entity/vkentitystateloaderdata.refl.hh>

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

