#pragma once


#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Core/Collection.hh>

struct IRenderer;
struct IRenderTarget;
struct ISampler;
struct IShaderAttribute;
struct ITexture2DArray;
class vkCamera;
class vkDirectionalLight;
class vkGBuffer;
class vkGeometryNode;
class vkLight;
class vkNode;
class vkPointLight;
class vkProgramGL4;
class RendererGL4;
class vkRenderTargetGL4;
class vkResourceLocator;


class vkLightRendererGL4
{
public:
  vkLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkLightRendererGL4();

  virtual void Render(vkNode *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target) = 0;

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
  vkCollection<vkGeometryNode*> m_geometries;
  RendererGL4 *m_renderer;
  float m_mapBias;

  // shadow buffer
  vkRenderTargetGL4 *m_shadowBuffer;
  vkVector2f m_shadowIntensity;

  ISampler *m_depthSampler;
};


class vkDirectionalLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkDirectionalLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkDirectionalLightRendererGL4();

  virtual void Render(vkNode *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkNode *node, const vkCamera *camera, const vkDirectionalLight *light);
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




class vkPointLightRendererGL4 : public vkLightRendererGL4
{
public:
  vkPointLightRendererGL4(RendererGL4 *renderer);
  virtual ~vkPointLightRendererGL4();

  virtual void Render(vkNode *node, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkNode *node, const vkPointLight *light);
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

VK_FORCEINLINE vkRenderTargetGL4 *vkLightRendererGL4::GetShadowBuffer()
{
  return m_shadowBuffer;
}