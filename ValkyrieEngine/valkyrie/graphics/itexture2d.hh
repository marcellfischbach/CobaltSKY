#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.refl.hh>

CS_INTERFACE();
struct CSE_API iTexture2D : public virtual CS_SUPER(iTexture)
{
  CS_CLASS_GEN;
  iTexture2D() : iTexture() { }

  virtual ~iTexture2D() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetNumberOfLODs() const = 0;

  virtual bool CopyData(csUInt8 lod, csPixelFormat format, const void *data) = 0;

};
