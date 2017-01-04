#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgsub.refl.hh>

#define VK_SUB_NAME "Math/Sub"

VK_CLASS()
class VKE_API vkSGSub : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGSub();
  virtual ~vkSGSub() { }

  virtual bool Validate();

};
