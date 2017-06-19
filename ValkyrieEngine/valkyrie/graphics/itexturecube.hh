#pragma once
#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexturecube.refl.hh>


CS_INTERFACE();
struct CSE_API iTextureCube : public virtual CS_SUPER(iTexture)
{
  CS_CLASS_GEN;
  iTextureCube() : iTexture() { }

  virtual ~iTextureCube() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetDepth() const = 0;

  virtual bool CopyData(csTextureCubeFace face, csUInt8 lod, csPixelFormat format, const void *data) = 0;

};