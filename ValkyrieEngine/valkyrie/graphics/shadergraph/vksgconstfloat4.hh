#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgconstfloat4.refl.hh>



#define VK_CONST_FLOAT4_NAME "Const/Float4"


VK_CLASS()
class VKE_API vkSGConstFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat4();
  virtual ~vkSGConstFloat4() { };
};