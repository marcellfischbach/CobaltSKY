#pragma once


#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Core/vkcollection.hh>
#include <Valkyrie/vkenums.hh>

struct IGraphics;
struct IRenderTarget;
struct ISampler;
struct IShaderAttribute;
struct ITexture2DArray;
class vkCamera;
class vkClipper;
class vkDirectionalLight;
class vkEntity;
class vkGBuffer;
class vkLight;
class vkPointLight;
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


class vkDirectionalLightRenderer : public vkLightRenderer
{
public:
  vkDirectionalLightRenderer(IGraphics *renderer);
  virtual ~vkDirectionalLightRenderer();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, vkCamera *camera, const vkDirectionalLight *light);
  void CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera);
  void CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &camInv, vkVector3f &min, vkVector3f &max) const;
  void UpdateProjectionMatrices();
  vkClipper *CreateClipper();

  vkMatrix4f m_shadowCam[3];
  vkMatrix4f m_shadowCamInv[3];
  vkMatrix4f m_shadowProj[3];
  vkMatrix4f m_shadowProjView[3];
  vkVector2f m_shadowNearFar[3];
  vkVector3f m_min[3];
  vkVector3f m_max[3];

  vkMatrix4f m_shadowCamAll;
  vkMatrix4f m_shadowCamInvAll;
  vkVector3f m_minAll;
  vkVector3f m_maxAll;

  vkVector3f m_distances;

  LightProgram m_programNoShadow;
  IShaderAttribute *m_attrLightDirectionNoShadow;
  void BindDirectionalLightNoShadow(vkDirectionalLight *directionalLight);

  LightProgram m_programPSSM;
  IShaderAttribute *m_attrLightDirectionPSSM;
  IShaderAttribute *m_attrDisancesPSSM;
  IShaderAttribute *m_attrShadowMatsProjView;
  IShaderAttribute *m_attrShadowMatsProj;
  IShaderAttribute *m_attrShadowMatsView;
  IShaderAttribute *m_attrShadowMap;
  IShaderAttribute *m_attrShadowColorMap;
  IShaderAttribute *m_attrShadowMapSizeInv;
  IShaderAttribute *m_attrMapBias;
  IShaderAttribute *m_attrShadowIntensity;
  IShaderAttribute *m_attrShadowProjNearFar;
  ITexture2DArray *m_colorBuffer;
  ITexture2DArray *m_depthBuffer;
  ITexture2DArray *m_colorBufferBlur;
  void BindDirectionalLightPSSM(vkDirectionalLight *directionalLight);

};




class vkPointLightRenderer : public vkLightRenderer
{
public:
  vkPointLightRenderer(IGraphics *renderer);
  virtual ~vkPointLightRenderer();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, const vkPointLight *light);
  void CalcCubeMatrices(const vkPointLight *light);

  vkMatrix4f m_shadowCam[6];
  vkMatrix4f m_shadowProj[6];
  vkMatrix4f m_shadowProjView[6];
  vkVector2f m_shadowNearFar[6];

  LightProgram m_programNoShadow;
  IShaderAttribute *m_attrLightPositionNoShadow;
  IShaderAttribute *m_attrLightRangeNoShadow;
  void BindPointLightNo(vkPointLight *pointLight);


  LightProgram m_programCubeShadow;
  IShaderAttribute *m_attrLightPositionCubeShadow;
  IShaderAttribute *m_attrLightRangeCubeShadow;
  IShaderAttribute *m_attrShadowMats;
  IShaderAttribute *m_attrShadowMap;
  IShaderAttribute *m_attrMapBias;
  IShaderAttribute *m_attrShadowIntensity;
  ITexture2DArray *m_depthBuffer;
  void BindPointLightCubeShadow(vkPointLight *pointLight);

};

VK_FORCEINLINE IRenderTarget *vkLightRenderer::GetShadowBuffer()
{
  return m_shadowBuffer;
}
