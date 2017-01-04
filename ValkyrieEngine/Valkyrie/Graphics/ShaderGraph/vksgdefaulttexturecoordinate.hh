#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgnode.hh>
#include <Valkyrie/Graphics/ShaderGraph/vksgdefaulttexturecoordinate.refl.hh>

#define VK_DEFAULT_TEXTURE_COORDINATE_NAME "Texture/DefaultTextureCoordinate"

VK_CLASS()
class VKE_API vkSGDefaultTextureCoordinate : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDefaultTextureCoordinate();
  virtual ~vkSGDefaultTextureCoordinate() { }

};
