#pragma once

#include <valkyrie/graphics/deferred/vklightrenderer.hh>

struct ITexture2DArray;
class vkDirectionalLight;

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
