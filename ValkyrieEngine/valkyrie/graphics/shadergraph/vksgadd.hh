
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgadd.refl.hh>

#define VK_ADD_NAME "Math/Add"

VK_CLASS()
class VKE_API vkSGAdd : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGAdd();
  virtual ~vkSGAdd() { }

  virtual bool Validate();

};
