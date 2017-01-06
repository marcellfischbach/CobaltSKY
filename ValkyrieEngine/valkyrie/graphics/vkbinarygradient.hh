#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/itexture2darray.hh>

class VKE_API vkBinaryGradient
{
public:
  virtual ~vkBinaryGradient();

  
  static ITexture2D *GetBinaryGradientLevel1();
  static ITexture2DArray *GetBinaryGradient();

private:
  vkBinaryGradient();

  
  static ITexture2D *static_texture1;
  static ITexture2DArray *static_textureArray;
};