#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat2.refl.hh>


#define VK_VAR_FLOAT2_NAME "Var/Float2"

VK_CLASS()
class VKE_API vkSGVarFloat2 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat2();
  virtual ~vkSGVarFloat2() { }
};