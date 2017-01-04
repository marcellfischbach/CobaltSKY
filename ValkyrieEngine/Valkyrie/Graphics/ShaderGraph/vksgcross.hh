
#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgcross.refl.hh>

#define VK_CROSS_NAME "Math/Cross"

VK_CLASS()
class VKE_API vkSGCross : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGCross();
  virtual ~vkSGCross() { }

  virtual bool Validate();

};
