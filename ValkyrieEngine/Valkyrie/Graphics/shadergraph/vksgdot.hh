#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksgdot.refl.hh>

#define VK_DOT_NAME "Math/Dot"

VK_CLASS()
class VKE_API vkSGDot : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDot();
  virtual ~vkSGDot() { }

  virtual bool Validate();

};
