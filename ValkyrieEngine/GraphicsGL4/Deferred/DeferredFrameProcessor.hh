
#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <GraphicsGL4/Deferred/DefaultCollector.hh>
#include <GraphicsGL4/Deferred/DeferredFrameProcessor.refl.hh>

struct IShader;
class vkGBuffer;
class vkGraphicsGL4;
class vkLight;
class vkLightvkGraphicsGL4;
class vkGeometryMesh;
class vkParticleRendererGL4;

VK_INTERFACE();
class VKGL4_API vkDeferredFrameProcessor : public virtual IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(vkGraphicsGL4 *renderer);
  virtual ~vkDeferredFrameProcessor();

  bool Resize(vkUInt16 width, vkUInt16 height);
  bool Initialize();

  void SetPostProcessor(vkPostProcessor *processor);

  IRenderTarget *Render(vkEntity *root, vkCamera *camera, IRenderTarget *target);

private:
  void RenderGBuffer(vkEntity *root);
  void RenderForward (vkRenderState *renderState);

private:
  vkCollection<vkRenderState*> m_renderStatesDeferred;
  vkCollection<vkRenderState*> m_renderStatesForward;
  vkCollection<vkRenderState*> m_renderStatesForwardTransprent;
  vkCollection<vkRenderState*> m_renderStatesParticles;
  vkCollection<vkLightState*> m_lightStates;
  vkGraphicsGL4 *m_renderer;

  vkPostProcessor *m_postProcessor;

  vkGBuffer *m_gbuffer;

  IShader *m_simplePresentShader;
  IShader *m_directionLightShader;

  vkLightvkGraphicsGL4 *m_lightRenderers[eLT_Count];
  vkParticleRendererGL4 *m_particleRenderer;
};
