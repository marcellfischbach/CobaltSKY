#pragma once


#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/core/vkcollection.hh>
#include <valkyrie/vkenums.hh>

struct iGraphics;
struct iRenderTarget;
struct iSampler;
struct iShaderAttribute;
class vkCamera;
class vkClipper;
class vkEntity;
class vkGBuffer;
class vkLight;
struct iShader;
class vkRenderState;
struct iRenderTarget;
class vkResourceLocator;


class vkLightRenderer
{
public:
  vkLightRenderer(iGraphics *renderer);
  virtual ~vkLightRenderer();

  virtual void Render(vkEntity *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, iRenderTarget *target) = 0;

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

  void InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator);


  void BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer);
  void BindLight(LightProgram &lightProgram, vkLight *light);

protected:
  void CalcShadowIntensity(const vkLight *light);
  vkCollection<vkRenderState*> m_renderStates[eRQ_COUNT];
  iGraphics *m_renderer;
  float m_mapBias;

  // shadow buffer
  float m_shadowBufferSize;
  iRenderTarget *m_shadowBuffer;
  vkVector2f m_shadowIntensity;

  iSampler *m_depthSampler;
};




VK_FORCEINLINE iRenderTarget *vkLightRenderer::GetShadowBuffer()
{
  return m_shadowBuffer;
}
