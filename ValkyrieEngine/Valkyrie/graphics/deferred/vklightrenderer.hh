#pragma once


#include <Valkyrie/Math/vkmatrix.hh>
#include <Valkyrie/core/vkcollection.hh>
#include <Valkyrie/vkenums.hh>

struct IGraphics;
struct IRenderTarget;
struct ISampler;
struct IShaderAttribute;
class vkCamera;
class vkClipper;
class vkEntity;
class vkGBuffer;
class vkLight;
struct IShader;
class vkRenderState;
struct IRenderTarget;
class vkResourceLocator;


class vkLightRenderer
{
public:
  vkLightRenderer(IGraphics *renderer);
  virtual ~vkLightRenderer();

  virtual void Render(vkEntity *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target) = 0;

  IRenderTarget *GetShadowBuffer();

protected:

  struct GBufferAttribs
  {
    IShaderAttribute *attrDiffuseRoughness;
    IShaderAttribute *attrNormalLightMode;
    IShaderAttribute *attrEmissiveMetallic;
    IShaderAttribute *attrSSSSpecular;
    IShaderAttribute *attrDepth;
  };

  struct LightProgram
  {
    IShader *program;
    GBufferAttribs gbuffer;
    IShaderAttribute *attrColor;
    IShaderAttribute *attrEnergy;
  };

  void InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator);


  void BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer);
  void BindLight(LightProgram &lightProgram, vkLight *light);

protected:
  void CalcShadowIntensity(const vkLight *light);
  vkCollection<vkRenderState*> m_renderStates[eRQ_COUNT];
  IGraphics *m_renderer;
  float m_mapBias;

  // shadow buffer
  float m_shadowBufferSize;
  IRenderTarget *m_shadowBuffer;
  vkVector2f m_shadowIntensity;

  ISampler *m_depthSampler;
};




VK_FORCEINLINE IRenderTarget *vkLightRenderer::GetShadowBuffer()
{
  return m_shadowBuffer;
}
