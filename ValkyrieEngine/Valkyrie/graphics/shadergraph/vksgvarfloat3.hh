#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat3.refl.hh>


#define VK_VAR_FLOAT3_NAME "Var/Float3"

VK_CLASS()
class VKE_API vkSGVarFloat3 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat3();
  virtual ~vkSGVarFloat3() { }
};