#pragma once
#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/graphics/itexture.hh>
#include <Valkyrie/graphics/itexturecube.refl.hh>


VK_INTERFACE();
struct VKE_API ITextureCube : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITextureCube() : ITexture() { }

  virtual ~ITextureCube() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetDepth() const = 0;

  virtual bool CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};