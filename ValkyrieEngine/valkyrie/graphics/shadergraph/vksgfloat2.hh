#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat2.refl.hh>

#define VK_FLOAT2_NAME "Assemble/Float2"

VK_CLASS()
class VKE_API vkSGFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat2();
  virtual ~vkSGFloat2() { };
};
