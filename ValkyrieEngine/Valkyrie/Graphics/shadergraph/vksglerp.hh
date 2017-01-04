#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/shadergraph/vksgnode.hh>
#include <Valkyrie/Graphics/shadergraph/vksglerp.refl.hh>

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
