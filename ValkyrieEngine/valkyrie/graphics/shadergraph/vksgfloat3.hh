#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgfloat3.refl.hh>

#define VK_FLOAT3_NAME "Assemble/Float3"

VK_CLASS()
class VKE_API vkSGFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat3();
  virtual ~vkSGFloat3() { };
};
