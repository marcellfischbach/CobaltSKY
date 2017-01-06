#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgdot.refl.hh>

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
