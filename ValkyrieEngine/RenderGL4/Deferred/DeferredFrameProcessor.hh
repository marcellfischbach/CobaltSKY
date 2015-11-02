
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <RenderGL4/Deferred/DeferredFrameProcessor.refl.hh>

class vkGeometryNode;
class RendererGL4;

VK_INTERFACE();
class VKGL4_API vkDeferredFrameProcessor : public virtual IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(RendererGL4 *renderer);
  virtual ~vkDeferredFrameProcessor();

  void Render(vkNode *node, IRenderTarget *target);

private:
  vkCollection<vkGeometryNode*> m_geometries;
  RendererGL4 *m_renderer;
};