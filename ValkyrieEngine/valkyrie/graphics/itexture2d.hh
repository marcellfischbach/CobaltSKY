#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2d.refl.hh>

VK_INTERFACE();
struct VKE_API iTexture2D : public virtual VK_SUPER(iTexture)
{
  VK_CLASS_GEN;
  iTexture2D() : iTexture() { }

  virtual ~iTexture2D() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetNumberOfLODs() const = 0;

  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};
