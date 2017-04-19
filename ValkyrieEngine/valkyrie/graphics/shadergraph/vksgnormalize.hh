#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgnormalize.refl.hh>

#define VK_NORMALIZE_NAME "Math/Normalize"

VK_CLASS()
class VKE_API vkSGNormalize : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGNormalize();
  virtual ~vkSGNormalize() { }

  virtual bool Validate();

};
