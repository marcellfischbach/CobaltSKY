
#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/core/vkcollection.hh>
#include <valkyrie/graphics/iframeprocessor.hh>
#include <valkyrie/graphics/deferred/vkdefaultcollector.hh>
#include <valkyrie/graphics/deferred/vkdeferredframeprocessor.refl.hh>

struct iShader;
class vkGBuffer;
struct iGraphics;
class vkLight;
class vkLightRenderer;
class vkGeometryMesh;
class vkParticleRenderer;

VK_INTERFACE()
class VKE_API vkDeferredFrameProcessor : public  VK_SUPER(iFrameProcessor)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDeferredFrameProcessor(iGraphics *renderer);
  virtual ~vkDeferredFrameProcessor();

  bool Resize(vkUInt16 width, vkUInt16 height);
  bool Initialize();

  void SetPostProcessor(vkPostProcessor *processor);

  iRenderTarget *Render(vkEntity *root, vkCamera *camera, iRenderTarget *target);

private:
  void RenderGBuffer(vkEntity *root);
  void RenderForward (vkRenderState *renderState);

private:
  vkCollection<vkRenderState*> m_renderStates[eRQ_COUNT];
  vkCollection<vkLightState*> m_lightStates;
  iGraphics *m_renderer;

  vkPostProcessor *m_postProcessor;

  vkGBuffer *m_gbuffer;

  iShader *m_simplePresentShader;
  iShader *m_directionLightShader;

  vkLightRenderer *m_lightRenderers[eLT_Count];
  vkParticleRenderer *m_particleRenderer;
};
