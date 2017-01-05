#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <Valkyrie/graphics/shadergraph/vksgvarfloat4.refl.hh>


#define VK_VAR_FLOAT4_NAME "Var/Float4"

VK_CLASS()
class VKE_API vkSGVarFloat4 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat4();
  virtual ~vkSGVarFloat4() { }
};