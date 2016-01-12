#pragma once


#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Core/Collection.hh>

struct IGraphics;
struct IRenderTarget;
struct ISampler;
struct IShaderAttribute;
struct ITexture2DArray;
class vkCamera;
class vkDirectionalLight;
class vkEntity;
class vkGBuffer;
class vkLight;
class vkPointLight;
class vkProgramGL4;
class vkGraphicsGL4;
class vkRenderState;
class vkRenderTargetGL4;
class vkResourceLocator;


class vkLightvkGraphicsGL4
{
public:
  vkLightvkGraphicsGL4(vkGraphicsGL4 *renderer);
  virtual ~vkLightvkGraphicsGL4();

  virtual void Render(vkEntity *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target) = 0;

  vkRenderTargetGL4 *GetShadowBuffer();

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
    vkProgramGL4 *program;
    GBufferAttribs gbuffer;
    IShaderAttribute *attrColor;
    IShaderAttribute *attrEnergy;
  };

  void InitializeLightProgram(LightProgram *lightProgram, const vkResourceLocator &locator);


  void BindGBuffer(GBufferAttribs &attribs, vkGBuffer *gbuffer);
  void BindLight(LightProgram &lightProgram, vkLight *light);

protected:
  void CalcShadowIntensity(const vkLight *light);
  vkCollection<vkRenderState*> m_renderStates;
  vkGraphicsGL4 *m_renderer;
  float m_mapBias;

  // shadow buffer
  vkRenderTargetGL4 *m_shadowBuffer;
  vkVector2f m_shadowIntensity;

  ISampler *m_depthSampler;
};


class vkDirectionalLightvkGraphicsGL4 : public vkLightvkGraphicsGL4
{
public:
  vkDirectionalLightvkGraphicsGL4(vkGraphicsGL4 *renderer);
  virtual ~vkDirectionalLightvkGraphicsGL4();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, vkCamera *camera, const vkDirectionalLight *light);
  void CalcPSSMMatrices(const vkDirectionalLight *light, const vkCamera *camera);
  void CalcMatrix(const vkVector3f &dir, vkSize numPoints, vkVector3f *points, vkMatrix4f &cam, vkMatrix4f &proj) const;

  vkMatrix4f m_shadowCam[3];
  vkMatrix4f m_shadowProj[3];
  vkMatrix4f m_shadowProjView[3];

  vkVector3f m_distances;

  LightProgram m_programNoShadow;
  IShaderAttribute *m_attrLightDirectionNoShadow;
  void BindDirectionalLightNoShadow(vkDirectionalLight *directionalLight);

  LightProgram m_programPSSM;
  IShaderAttribute *m_attrLightDirectionPSSM;
  IShaderAttribute *m_attrDisancesPSSM;
  IShaderAttribute *m_attrShadowMats;
  IShaderAttribute *m_attrShadowMap;
  IShaderAttribute *m_attrMapBias;
  IShaderAttribute *m_attrShadowIntensity;
  ITexture2DArray *m_depthBuffer;
  void BindDirectionalLightPSSM(vkDirectionalLight *directionalLight);

};




class vkPointLightvkGraphicsGL4 : public vkLightvkGraphicsGL4
{
public:
  vkPointLightvkGraphicsGL4(vkGraphicsGL4 *renderer);
  virtual ~vkPointLightvkGraphicsGL4();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, const vkPointLight *light);
  void CalcCubeMatrices(const vkPointLight *light);

  vkMatrix4f m_shadowCam[6];
  vkMatrix4f m_shadowProj[6];
  vkMatrix4f m_shadowProjView[6];

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

VK_FORCEINLINE vkRenderTargetGL4 *vkLightvkGraphicsGL4::GetShadowBuffer()
{
  return m_shadowBuffer;
}