#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat2.refl.hh>



#define VK_CONST_FLOAT2_NAME "Const/Float2"


VK_CLASS()
class VKE_API vkSGConstFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat2();
  virtual ~vkSGConstFloat2() { };
};