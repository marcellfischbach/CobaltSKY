#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsplitfloat4.refl.hh>


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
