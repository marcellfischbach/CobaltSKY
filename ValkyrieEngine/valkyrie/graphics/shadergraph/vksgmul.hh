#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgmul.refl.hh>

#define VK_MUL_NAME "Math/Mul"

VK_CLASS()
class VKE_API vkSGMul : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGMul();
  virtual ~vkSGMul() { }

  virtual bool Validate();

};
