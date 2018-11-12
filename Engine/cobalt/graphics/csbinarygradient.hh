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

  
  static csTexture2DWrapper *GetBinaryGradientLevel1();
  static csTexture2DArrayWrapper *GetBinaryGradient();

private:
  csBinaryGradient();

  
  static csTexture2DWrapper *static_texture1;
  static csTexture2DArrayWrapper *static_textureArray;
};
