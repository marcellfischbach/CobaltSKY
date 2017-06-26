#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2darray.refl.hh>

CS_INTERFACE();
struct CSE_API iTexture2DArray : public virtual CS_SUPER(iTexture)
{
  CS_CLASS_GEN;
  iTexture2DArray() : iTexture() { }

  virtual ~iTexture2DArray() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetLayers() const = 0;


  virtual bool CopyData(csUInt8 lod, csPixelFormat format, const void *data) = 0;
  virtual bool CopyData(csUInt16 layer, csUInt8 lod, csPixelFormat format, const void *data) = 0;

};