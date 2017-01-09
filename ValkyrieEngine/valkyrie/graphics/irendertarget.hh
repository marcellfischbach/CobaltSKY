#pragma once

#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkexport.hh>
#include <valkyrie/graphics/itexture.hh>
#include <valkyrie/graphics/irendertarget.refl.hh>


VK_INTERFACE();
struct VKE_API iRenderTarget : public iObject
{
  VK_CLASS_GEN;

  virtual vkUInt16 GetWidth() const = 0;
  virtual vkUInt16 GetHeight() const = 0;

  virtual void AddColorTexture(iTexture *color) = 0;
  virtual void SetDepthTexture(iTexture *depth) = 0;
  virtual void SetDepthBuffer(vkUInt16 width, vkUInt16 height) = 0;
  virtual void Initialize(vkUInt16 width, vkUInt16 height) = 0;
  virtual bool Finilize() = 0;

  virtual iTexture *GetColorBuffer(vkUInt8 buffer) const = 0;
  virtual iTexture *GetDepthBuffer() const = 0;

};