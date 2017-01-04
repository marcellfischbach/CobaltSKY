#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat2.refl.hh>



#define VK_CONST_FLOAT2_NAME "Const/Float2"


VK_CLASS()
class VKE_API vkSGConstFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat2();
  virtual ~vkSGConstFloat2() { };
};