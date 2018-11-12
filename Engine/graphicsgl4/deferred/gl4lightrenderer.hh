#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/csenums.hh>

struct iGraphics;
struct iRenderTarget;
class csSamplerWrapper;
struct iShaderAttribute;
class csCamera;
class csClipper;
class csEntity;
class csGBufferGL4;
class csLight;
class csShaderWrapper;
class csRenderState;
struct iRenderTarget;
class csResourceLocator;


class csLightRendererGL4
{
public:
  csLightRendererGL4(iGraphics *renderer);
  virtual ~csLightRendererGL4();

  virtual void Render(csEntity *node, csCamera *camera, csLight *light, csGBufferGL4 *gbuffer, iRenderTarget *target) = 0;

  iRenderTarget *GetShadowBuffer();

protected:

  struct GBufferAttribs
  {
    iShaderAttribute *attrDiffuseRoughness;
    iShaderAttribute *attrNormalLightMode;
    iShaderAttribute *attrEmissiveMetallic;
    iShaderAttribute *attrSSSSpecular;
    iShaderAttribute *attrDepth;
  };

  struct LightProgram
  {
    csShaderWrapper *program;
    GBufferAttribs gbuffer;
    iShaderAttribute *attrColor;
    iShaderAttribute *attrEnergy;
  };

  void InitializeLightProgram(LightProgram *lightProgram, const csResourceLocator &locator);


  void BindGBuffer(GBufferAttribs &attribs, csGBufferGL4 *gbuffer);
  void BindLight(LightProgram &lightProgram, csLight *light);

protected:
  void CalcShadowIntensity(const csLight *light);
  csCollection<csRenderState*> m_renderStates[eRQ_COUNT];
  iGraphics *m_renderer;
  float m_mapBias;

  // shadow buffer
  csUInt16 m_shadowBufferSize;
  iRenderTarget *m_shadowBuffer;
  csVector2f m_shadowIntensity;

  csSamplerWrapper *m_depthSampler;
};




CS_FORCEINLINE iRenderTarget *csLightRendererGL4::GetShadowBuffer()
{
  return m_shadowBuffer;
}
