#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat3.refl.hh>



#define VK_CONST_FLOAT3_NAME "Const/Float3"


VK_CLASS()
class VKE_API vkSGConstFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3();
  virtual ~vkSGConstFloat3() { };
}; 