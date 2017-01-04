#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsplitfloat3.refl.hh>

#define VK_SPLIT_FLOAT3_NAME "Assemble/SplitFloat3"

VK_CLASS()
class VKE_API vkSGSplitFloat3 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat3();
  virtual ~vkSGSplitFloat3() { }

  virtual bool Validate();
};
