#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat3.refl.hh>


#define VK_VAR_FLOAT3_NAME "Var/Float3"

VK_CLASS()
class VKE_API vkSGVarFloat3 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat3();
  virtual ~vkSGVarFloat3() { }
};