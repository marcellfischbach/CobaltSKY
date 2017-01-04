#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgfloat4.refl.hh>

#define VK_FLOAT4_NAME "Assemble/Float4"

VK_CLASS()
class VKE_API vkSGFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat4();
  virtual ~vkSGFloat4() { };
};
