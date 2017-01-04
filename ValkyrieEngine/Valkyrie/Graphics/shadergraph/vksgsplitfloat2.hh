#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgsplitfloat2.refl.hh>

#define VK_SPLIT_FLOAT2_NAME "Assemble/SplitFloat2"



VK_CLASS()
class VKE_API vkSGSplitFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSplitFloat2();
  virtual ~vkSGSplitFloat2() { }

  virtual bool Validate();
};
