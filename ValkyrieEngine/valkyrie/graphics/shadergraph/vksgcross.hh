
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgcross.refl.hh>

#define VK_CROSS_NAME "Math/Cross"

VK_CLASS()
class VKE_API vkSGCross : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGCross();
  virtual ~vkSGCross() { }

  virtual bool Validate();

};
