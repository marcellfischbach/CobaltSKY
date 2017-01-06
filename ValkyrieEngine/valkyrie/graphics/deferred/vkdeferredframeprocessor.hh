
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkcollection.hh>
#include <valkyrie/graphics/iframeprocessor.hh>
#include <valkyrie/graphics/deferred/vkdefaultcollector.hh>
#include <valkyrie/graphics/deferred/vkdeferredframeprocessor.refl.hh>

struct IShader;
class vkGBuffer;
struct IGraphics;
class vkLight;
class vkLightRenderer;
class vkGeometryMesh;
class vkParticleRenderer;

VK_INTERFACE()
class VKE_API vkDeferredFrameProcessor : public  VK_SUPER(IFrameProcessor)
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
  vkCollection<vkRenderState*> m_renderStates[eRQ_COUNT];
  vkCollection<vkLightState*> m_lightStates;
  IGraphics *m_renderer;

  vkPostProcessor *m_postProcessor;

  vkGBuffer *m_gbuffer;

  IShader *m_simplePresentShader;
  IShader *m_directionLightShader;

  vkLightRenderer *m_lightRenderers[eLT_Count];
  vkParticleRenderer *m_particleRenderer;
};