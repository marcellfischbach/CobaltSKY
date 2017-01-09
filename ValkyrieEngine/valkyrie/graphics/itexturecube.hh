#pragma once
#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexturecube.refl.hh>


VK_INTERFACE();
struct VKE_API iTextureCube : public virtual VK_SUPER(iTexture)
{
  VK_CLASS_GEN;
  iTextureCube() : iTexture() { }

  virtual ~iTextureCube() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetDepth() const = 0;

  virtual bool CopyData(vkTextureCubeFace face, vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};