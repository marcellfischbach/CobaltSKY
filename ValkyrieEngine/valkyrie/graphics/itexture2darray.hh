#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/itexture2darray.refl.hh>

VK_INTERFACE();
struct VKE_API ITexture2DArray : public virtual VK_SUPER(ITexture)
{
  VK_CLASS_GEN;
  ITexture2DArray() : ITexture() { }

  virtual ~ITexture2DArray() { }

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;
  virtual vkUInt16 GetLayers() const = 0;


  virtual bool CopyData(vkUInt8 lod, vkPixelFormat format, const void *data) = 0;
  virtual bool CopyData(vkUInt16 layer, vkUInt8 lod, vkPixelFormat format, const void *data) = 0;

};
