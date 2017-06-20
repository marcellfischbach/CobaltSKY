#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>

class CSE_API csBinaryGradient
{
public:
  virtual ~csBinaryGradient();

  
  static iTexture2D *GetBinaryGradientLevel1();
  static iTexture2DArray *GetBinaryGradient();

private:
  csBinaryGradient();

  
  static iTexture2D *static_texture1;
  static iTexture2DArray *static_textureArray;
};