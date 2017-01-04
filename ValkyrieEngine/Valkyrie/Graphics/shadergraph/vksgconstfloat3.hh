#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat3.refl.hh>



#define VK_CONST_FLOAT3_NAME "Const/Float3"


VK_CLASS()
class VKE_API vkSGConstFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat3();
  virtual ~vkSGConstFloat3() { };
}; 
