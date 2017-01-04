#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgresourcenode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgvarfloat1.refl.hh>


#define VK_VAR_FLOAT_NAME "Var/Float"

VK_CLASS()
class VKE_API vkSGVarFloat : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat();
  virtual ~vkSGVarFloat() { }
};