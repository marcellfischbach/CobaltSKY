#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgresourcenode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat2.refl.hh>


#define VK_VAR_FLOAT2_NAME "Var/Float2"

VK_CLASS()
class VKE_API vkSGVarFloat2 : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat2();
  virtual ~vkSGVarFloat2() { }
};