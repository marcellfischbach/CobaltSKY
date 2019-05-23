
#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/graphics/iframeprocessor.hh>
#include <cobalt/math/cscolor4f.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4deferredframeprocessor.refl.hh>



namespace cs
{
struct iGraphics;
struct iShader;
class GeometryMesh;
class GBufferGL4;
class Light;
class LightRendererGL4;
class ParticleRendererGL4;

CS_CLASS()
class CSGRAPHICSGL4_API DeferredFrameProcessorGL4 : public  CS_SUPER(cs::iFrameProcessor)
{
  CS_CLASS_GEN_OBJECT;
public:
  DeferredFrameProcessorGL4(cs::iGraphics *renderer);
  virtual ~DeferredFrameProcessorGL4();

  bool Resize(csUInt16 width, csUInt16 height);
  bool Initialize();

  void SetPostProcessor(cs::PostProcessor *processor);
  void SetClearColor(const cs::Color4f &clearColor);
  const cs::Color4f &GetClearColor() const;

  cs::iRenderTarget *Render(cs::Entity *root, cs::Camera *camera, cs::iRenderTarget *target);

private:
  void RenderGBuffer(cs::Entity *root);
  void RenderForward (cs::RenderState *renderState);

private:
  csUInt64 m_frameNo;
  cs::Collection<cs::RenderState*> m_renderStates[cs::eRQ_COUNT];
  cs::Collection<cs::LightState*> m_lightStates;
  cs::iGraphics *m_renderer;

  cs::PostProcessor *m_postProcessor;

  cs::GBufferGL4 *m_gbuffer;

  cs::Color4f m_clearColor;

  cs::iShader *m_simplePresentShader;
  cs::iShader *m_directionLightShader;

  cs::LightRendererGL4 *m_lightRenderers[cs::eLT_Count];
  cs::ParticleRendererGL4 *m_particleRenderer;
};

}
