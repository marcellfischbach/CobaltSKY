#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgfloat2.refl.hh>

#define VK_FLOAT2_NAME "Assemble/Float2"

VK_CLASS()
class VKE_API vkSGFloat2 : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGFloat2();
  virtual ~vkSGFloat2() { };
};
