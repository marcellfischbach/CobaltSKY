
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/iframeprocessor.refl.hh>


class vkCamera;
class vkNode;
class vkEntity;
class vkPostProcessor;
struct iRenderTarget;

VK_INTERFACE();
struct VKE_API iFrameProcessor : public iObject
{
  VK_CLASS_GEN;
  iFrameProcessor() : iObject() { }

  virtual bool Resize(vkUInt16 width, vkUInt16 height) = 0;
  virtual bool Initialize() = 0;

  virtual void SetPostProcessor(vkPostProcessor *processor) = 0;

  virtual iRenderTarget *Render(vkEntity *root, vkCamera *camera, iRenderTarget *target = 0) = 0;

};
