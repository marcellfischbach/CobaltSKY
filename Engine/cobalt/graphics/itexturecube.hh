#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/itexture.hh>
#include <cobalt/graphics/itexturecube.refl.hh>


namespace cs
{

CS_CLASS();
struct CSE_API iTextureCube : public virtual CS_SUPER(cs::iTexture)
{
  CS_CLASS_GEN;
  iTextureCube() : cs::iTexture() { }

  virtual ~iTextureCube() { }

  virtual csUInt16 GetWidth() const = 0;
  virtual csUInt16 GetHeight() const = 0;
  virtual csUInt16 GetDepth() const = 0;

  virtual bool CopyData(cs::eTextureCubeFace face, csUInt8 lod, cs::ePixelFormat format, const void* data) = 0;

};

CS_CLASS()
class CSE_API TextureCubeWrapper : public CS_SUPER(cs::TextureWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::iTextureCube, TextureCubeWrapper, cs::TextureWrapper);
};

}

