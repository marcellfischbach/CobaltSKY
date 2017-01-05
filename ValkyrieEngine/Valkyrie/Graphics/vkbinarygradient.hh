#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vktypes.hh>
#include <Valkyrie/Math/Color.hh>
#include <Valkyrie/Graphics/itexture2d.hh>
#include <Valkyrie/Graphics/itexture2darray.hh>

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