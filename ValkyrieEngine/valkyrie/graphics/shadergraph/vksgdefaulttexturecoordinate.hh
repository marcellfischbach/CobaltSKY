#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/shadergraph/vksgnode.hh>
#include <valkyrie/graphics/shadergraph/vksgdefaulttexturecoordinate.refl.hh>

#define VK_DEFAULT_TEXTURE_COORDINATE_NAME "Texture/DefaultTextureCoordinate"

VK_CLASS()
class VKE_API vkSGDefaultTextureCoordinate : public VK_SUPER(vkSGNode)
{
  VK_CLASS_GEN;
public:
  vkSGDefaultTextureCoordinate();
  virtual ~vkSGDefaultTextureCoordinate() { }

};
