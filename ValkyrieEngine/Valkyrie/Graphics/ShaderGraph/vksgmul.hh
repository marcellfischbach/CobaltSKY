#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgmul.refl.hh>

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
