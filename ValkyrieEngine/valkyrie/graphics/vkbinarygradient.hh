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

  
  static iTexture2D *GetBinaryGradientLevel1();
  static iTexture2DArray *GetBinaryGradient();

private:
  vkBinaryGradient();

  
  static iTexture2D *static_texture1;
  static iTexture2DArray *static_textureArray;
};