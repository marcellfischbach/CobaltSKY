#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/graphics/shadergraph/vksgfloat3.refl.hh>

#define VK_FLOAT3_NAME "Assemble/Float3"

VK_CLASS()
class VKE_API vkSGFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat3();
  virtual ~vkSGFloat3() { };
};
