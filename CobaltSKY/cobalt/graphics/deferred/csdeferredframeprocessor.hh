
#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csenums.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/graphics/iframeprocessor.hh>
#include <cobalt/graphics/deferred/csdefaultcollector.hh>
#include <cobalt/graphics/deferred/csdeferredframeprocessor.refl.hh>
#include <cobalt/math/cscolor4f.hh>


struct iShader;
class csGBuffer;
struct iGraphics;
class csLight;
class csLightRenderer;
class csGeometryMesh;
class csParticleRenderer;

CS_INTERFACE()
class CSE_API csDeferredFrameProcessor : public  CS_SUPER(iFrameProcessor)
{
  CS_CLASS_GEN_OBJECT;
public:
  csDeferredFrameProcessor(iGraphics *renderer);
  virtual ~csDeferredFrameProcessor();

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
  csCollection<csRenderState*> m_renderStates[eRQ_COUNT];
  csCollection<csLightState*> m_lightStates;
  iGraphics *m_renderer;

  csPostProcessor *m_postProcessor;

  csGBuffer *m_gbuffer;

  csColor4f m_clearColor;

  iShader *m_simplePresentShader;
  iShader *m_directionLightShader;

  csLightRenderer *m_lightRenderers[eLT_Count];
  csParticleRenderer *m_particleRenderer;
};
