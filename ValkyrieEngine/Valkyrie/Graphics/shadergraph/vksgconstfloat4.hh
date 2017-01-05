#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat4.refl.hh>



#define VK_CONST_FLOAT4_NAME "Const/Float4"


VK_CLASS()
class VKE_API vkSGConstFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGConstFloat4();
  virtual ~vkSGConstFloat4() { };
};