#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgresourcenode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgtexture2d.refl.hh>

#define VK_TEXTURE2D_NAME "Texture/Texture2D"

VK_CLASS()
class VKE_API vkSGTexture2D : public VK_SUPER(vkSGResourceNode)
{
  VK_CLASS_GEN;
public:
  vkSGTexture2D();
  virtual ~vkSGTexture2D() { }

};
