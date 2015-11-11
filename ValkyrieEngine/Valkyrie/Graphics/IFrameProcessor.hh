
#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IFrameProcessor.refl.hh>


class vkCamera;
class vkNode;
struct IRenderTarget;

VK_INTERFACE();
struct VKE_API IFrameProcessor : public virtual IObject
{
  VK_CLASS_GEN;
  IFrameProcessor() : IObject() { }

  virtual bool Initialize(vkUInt16 width, vkUInt16 height) = 0;

  virtual void Render(vkNode *root, const vkCamera *camera, IRenderTarget *target = 0) = 0;

};
