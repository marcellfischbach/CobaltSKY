
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <RenderGL4/Deferred/DeferredFrameProcessor.refl.hh>

VK_CLASS();
class VKGL4_API vkDeferredFrameProcessor : public virtual IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor();
  virtual ~vkDeferredFrameProcessor();

  void Render(vkNode *node, IRenderTarget *target);

};