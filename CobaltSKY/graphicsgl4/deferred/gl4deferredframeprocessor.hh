
#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/graphics/iframeprocessor.hh>
#include <cobalt/math/cscolor4f.hh>
#include <graphicsgl4/deferred/gl4defaultcollector.hh>
#include <graphicsgl4/deferred/gl4deferredframeprocessor.refl.hh>


struct iShader;
class csGBufferGL4;
struct iGraphics;
class csLight;
class csLightRendererGL4;
class csGeometryMesh;
class csParticleRendererGL4;

CS_INTERFACE()
class CSGRAPHICSGL4_API csDeferredFrameProcessorGL4 : public  CS_SUPER(iFrameProcessor)
{
  CS_CLASS_GEN_OBJECT;
public:
  csDeferredFrameProcessorGL4(iGraphics *renderer);
  virtual ~csDeferredFrameProcessorGL4();

  bool Resize(csUInt16 width, csUInt16 height);
  bool Initialize();

  void SetPostProcessor(csPostProcessor *processor);
  void SetClearColor(const csColor4f &clearColor);
  const csColor4f &GetClearColor() const;

  iRenderTarget *Render(csEntity *root, csCamera *camera, iRenderTarget *target);

private:
  void RenderGBuffer(csEntity *root);
  void RenderForward (csRenderState *renderState);

private:
  csUInt64 m_frameNo;
  csCollection<csRenderState*> m_renderStates[eRQ_COUNT];
  csCollection<csLightState*> m_lightStates;
  iGraphics *m_renderer;

  csPostProcessor *m_postProcessor;

  csGBufferGL4 *m_gbuffer;

  csColor4f m_clearColor;

  iShader *m_simplePresentShader;
  iShader *m_directionLightShader;

  csLightRendererGL4 *m_lightRenderers[eLT_Count];
  csParticleRendererGL4 *m_particleRenderer;
};
