#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.refl.hh>

VK_INTERFACE();
struct VKE_API ITexture2D : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITexture2D() : ITexture() { }

  virtual ~ITexture2D() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};
