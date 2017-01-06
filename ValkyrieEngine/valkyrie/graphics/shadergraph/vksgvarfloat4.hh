#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat4.refl.hh>


#define VK_VAR_FLOAT4_NAME "Var/Float4"

VK_CLASS()
class VKE_API vkSGVarFloat4 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat4();
  virtual ~vkSGVarFloat4() { }
};