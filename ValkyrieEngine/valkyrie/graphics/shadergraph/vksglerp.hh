#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksglerp.refl.hh>

#define VK_LERP_NAME "Math/Lerp"

VK_CLASS()
class VKE_API vkSGLerp : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGLerp();
  virtual ~vkSGLerp() { }

  virtual bool Validate();

};
