
#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Core/Collection.hh>
#include <Valkyrie/Graphics/IFrameProcessor.hh>
#include <Valkyrie/Graphics/Deferred/DefaultCollector.hh>
#include <Valkyrie/Graphics/Deferred/DeferredFrameProcessor.refl.hh>

struct IShader;
class vkGBuffer;
struct IGraphics;
class vkLight;
class vkLightRenderer;
class vkGeometryMesh;
class vkParticleRenderer;

VK_INTERFACE()
class VKE_API vkDeferredFrameProcessor : public  IFrameProcessor
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(IGraphics *renderer);
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
  IGraphics *m_renderer;

  vkPostProcessor *m_postProcessor;

  vkGBuffer *m_gbuffer;

  IShader *m_simplePresentShader;
  IShader *m_directionLightShader;

  vkLightRenderer *m_lightRenderers[eLT_Count];
  vkParticleRenderer *m_particleRenderer;
};
