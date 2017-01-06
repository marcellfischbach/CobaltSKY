#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgsplitfloat2.refl.hh>

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
