#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat1.refl.hh>


#define VK_CONST_FLOAT_NAME "Const/Float"


VK_CLASS()
class VKE_API vkSGConstFloat : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat();
  virtual ~vkSGConstFloat() { };
};