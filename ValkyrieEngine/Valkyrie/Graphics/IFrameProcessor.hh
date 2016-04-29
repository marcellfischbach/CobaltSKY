
#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IFrameProcessor.refl.hh>


class vkCamera;
class vkNode;
class vkEntity;
class vkPostProcessor;
struct IRenderTarget;

VK_INTERFACE();
struct VKE_API IFrameProcessor : public virtual IObject
{
  VK_CLASS_GEN;
  IFrameProcessor() : IObject() { }

  virtual bool Resize(vkUInt16 width, vkUInt16 height) = 0;
  virtual bool Initialize() = 0;

  virtual void SetPostProcessor(vkPostProcessor *processor) = 0;

  virtual IRenderTarget *Render(vkEntity *root, vkCamera *camera, IRenderTarget *target = 0) = 0;

};
