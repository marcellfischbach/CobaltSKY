#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2darray.refl.hh>

namespace cs
{

CS_CLASS();
struct CSE_API iTexture2DArray : public virtual CS_SUPER(cs::iTexture)
{
  CS_CLASS_GEN;
  iTexture2DArray() : cs::iTexture() { }

  virtual ~iTexture2DArray() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetLayers() const = 0;


  virtual bool CopyData(csUInt8 lod, cs::ePixelFormat format, const void* data) = 0;
  virtual bool CopyData(csUInt16 layer, csUInt8 lod, cs::ePixelFormat format, const void* data) = 0;

};

CS_CLASS()
class CSE_API Texture2DArrayWrapper : public CS_SUPER(cs::TextureWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::iTexture2DArray, Texture2DArrayWrapper, cs::TextureWrapper);
};

}