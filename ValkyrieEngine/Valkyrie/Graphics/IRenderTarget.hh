#pragma once

#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/ITexture.hh>
#include <Valkyrie/Graphics/IRenderTarget.refl.hh>


VK_INTERFACE();
struct VKE_API IRenderTarget : public virtual IObject
{
  VK_CLASS_GEN;

  virtual void AddColorTexture(ITexture *color) = 0;
  virtual void SetDepthTexture(ITexture *depth) = 0;
  virtual void SetDepthBuffer(vkUInt16 width, vkUInt16 height) = 0;
  virtual void Initialize() = 0;
  virtual bool Finilize() = 0;

  virtual ITexture *GetColorBuffer(vkUInt8 buffer) const = 0;
  virtual ITexture *GetDepthBuffer() const = 0;

};