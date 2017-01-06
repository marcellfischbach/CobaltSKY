#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat3.refl.hh>

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
