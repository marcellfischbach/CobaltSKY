#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Graphics/ITexture.hh>

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
