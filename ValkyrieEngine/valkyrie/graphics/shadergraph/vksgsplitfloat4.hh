#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat4.refl.hh>


#define VK_SPLIT_FLOAT4_NAME "Assemble/SplitFloat4"


VK_CLASS()
class VKE_API vkSGSplitFloat4 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat4();
  virtual ~vkSGSplitFloat4() { }

  virtual bool Validate();
};