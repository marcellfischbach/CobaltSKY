#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat4.refl.hh>

#define VK_FLOAT4_NAME "Assemble/Float4"

VK_CLASS()
class VKE_API vkSGFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat4();
  virtual ~vkSGFloat4() { };
};
