#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/itexture2darray.hh>

namespace cs
{

class CSE_API BinaryGradient
{
public:
  virtual ~BinaryGradient();


  static cs::Texture2DWrapper* GetBinaryGradientLevel1();
  static cs::Texture2DArrayWrapper* GetBinaryGradient();

private:
  BinaryGradient();


  static cs::Texture2DWrapper* static_texture1;
  static cs::Texture2DArrayWrapper* static_textureArray;
};

}
