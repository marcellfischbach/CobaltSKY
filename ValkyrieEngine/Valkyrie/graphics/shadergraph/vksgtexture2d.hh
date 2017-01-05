#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <Valkyrie/graphics/shadergraph/vksgtexture2d.refl.hh>

#define VK_TEXTURE2D_NAME "Texture/Texture2D"

VK_CLASS()
class VKE_API vkSGTexture2D : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2D();
  virtual ~vkSGTexture2D() { }

};