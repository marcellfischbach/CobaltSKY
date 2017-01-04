#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vktypes.hh>
#include <Valkyrie/Math/Color.hh>

struct ITexture2D;
struct ITexture2DArray;

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