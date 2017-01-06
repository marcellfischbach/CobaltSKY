#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgsub.refl.hh>

#define VK_SUB_NAME "Math/Sub"

VK_CLASS()
class VKE_API vkSGSub : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSub();
  virtual ~vkSGSub() { }

  virtual bool Validate();

};
