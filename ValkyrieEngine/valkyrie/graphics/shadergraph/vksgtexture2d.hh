#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <valkyrie/graphics/shadergraph/vksgtexture2d.refl.hh>

#define VK_TEXTURE2D_NAME "Texture/Texture2D"

VK_CLASS()
class VKE_API vkSGTexture2D : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2D();
  virtual ~vkSGTexture2D() { }

};