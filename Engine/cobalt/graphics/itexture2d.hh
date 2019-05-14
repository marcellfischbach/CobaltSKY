#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexture2d.refl.hh>

CS_CLASS();
struct CSE_API iTexture2D : public virtual CS_SUPER(iTexture)
{
  CS_CLASS_GEN;
  iTexture2D() : iTexture() { }

  virtual ~iTexture2D() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetNumberOfLODs() const = 0;

  virtual bool CopyData(csUInt8 lod, csPixelFormat format, const void *data) = 0;
  virtual bool ReadData(csUInt8 lod, csPixelFormat format, unsigned bufferSize, void *data, unsigned &dataSize) = 0;
};

CS_CLASS()
class CSE_API csTexture2DWrapper : public CS_SUPER(csTextureWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(iTexture2D, csTexture2DWrapper, csTextureWrapper);
};
