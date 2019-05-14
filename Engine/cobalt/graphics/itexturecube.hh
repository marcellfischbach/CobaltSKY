#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexturecube.refl.hh>


CS_CLASS();
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

CS_CLASS()
class CSE_API csTextureCubeWrapper : public CS_SUPER(csTextureWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(iTextureCube, csTextureCubeWrapper, csTextureWrapper);
};
