#pragma once


#include <cobalt/math/csmatrix.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/csenums.hh>

struct iGraphics;
struct iRenderTarget;
struct iSampler;
struct iShaderAttribute;
class csCamera;
class csClipper;
class csEntity;
class csGBuffer;
class csLight;
struct iShader;
class csRenderState;
struct iRenderTarget;
class csResourceLocator;


class csLightRenderer
{
public:
  csLightRenderer(iGraphics *renderer);
  virtual ~csLightRenderer();

  virtual void Render(csEntity *node, csCamera *camera, csLight *light, csGBuffer *gbuffer, iRenderTarget *target) = 0;

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
    iShader *program;
    GBufferAttribs gbuffer;
    iShaderAttribute *attrColor;
    iShaderAttribute *attrEnergy;
  };

  void InitializeLightProgram(LightProgram *lightProgram, const csResourceLocator &locator);


  void BindGBuffer(GBufferAttribs &attribs, csGBuffer *gbuffer);
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

  iSampler *m_depthSampler;
};




CS_FORCEINLINE iRenderTarget *csLightRenderer::GetShadowBuffer()
{
  return m_shadowBuffer;
}
