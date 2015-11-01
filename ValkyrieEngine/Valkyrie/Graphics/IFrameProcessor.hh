
#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IFrameProcessor.refl.hh>


class vkNode;
struct IRenderTarget;

VK_INTERFACE();
struct VKE_API IFrameProcessor : public virtual IObject
{
  VK_CLASS_GEN;

  virtual void Render(vkNode *root, IRenderTarget *target = 0) = 0;

};
