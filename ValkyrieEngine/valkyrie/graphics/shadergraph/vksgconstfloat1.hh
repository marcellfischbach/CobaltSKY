#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat1.refl.hh>


#define VK_CONST_FLOAT_NAME "Const/Float"


VK_CLASS()
class VKE_API vkSGConstFloat : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat();
  virtual ~vkSGConstFloat() { };
};