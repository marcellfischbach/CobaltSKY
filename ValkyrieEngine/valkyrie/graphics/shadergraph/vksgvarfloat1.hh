#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgvarfloat1.refl.hh>


#define VK_VAR_FLOAT_NAME "Var/Float"

VK_CLASS()
class VKE_API vkSGVarFloat : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:

  vkSGVarFloat();
  virtual ~vkSGVarFloat() { }
};