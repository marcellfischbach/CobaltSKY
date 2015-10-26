#pragma once

#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Export.hh>
#include <Valkyrie/Graphics/ITexture.hh>


VK_INTERFACE();
class VKE_API IFrameBuffer : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ITexture2D *GetColorBuffer(vkUInt8 buffer) const = 0;
  virtual ITexture2D *GetDepthBuffer(vkUInt8 buffer) const = 0;

};