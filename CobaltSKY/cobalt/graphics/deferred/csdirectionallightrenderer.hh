#pragma once

#include <cobalt/graphics/deferred/cslightrenderer.hh>

struct iTexture2DArray;
class csDirectionalLight;

class csDirectionalLightRenderer : public csLightRenderer
{
public:
  csDirectionalLightRenderer(iGraphics *renderer);
  virtual ~csDirectionalLightRenderer();

  virtual void Render(csEntity *root, csCamera *camera, csLight *light, csGBuffer *gbuffer, iRenderTarget *target);

private:
  void RenderShadow(csEntity *root, csCamera *camera, const csDirectionalLight *light);
  void CalcPSSMMatrices(const csDirectionalLight *light, const csCamera *camera);
  void CalcMatrix(const csVector3f &dir, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const;
  void UpdateProjectionMatrices();
  csClipper *CreateClipper();

  csMatrix4f m_shadowCam[3];
  csMatrix4f m_shadowCamInv[3];
  csMatrix4f m_shadowProj[3];
  csMatrix4f m_shadowProjView[3];
  csVector2f m_shadowNearFar[3];
  csVector3f m_min[3];
  csVector3f m_max[3];

  csMatrix4f m_shadowCamAll;
  csMatrix4f m_shadowCamInvAll;
  csVector3f m_minAll;
  csVector3f m_maxAll;

  csVector3f m_distances;

  LightProgram m_programNoShadow;
  iShaderAttribute *m_attrLightDirectionNoShadow;
  void BindDirectionalLightNoShadow(csDirectionalLight *directionalLight);

  LightProgram m_programPSSM;
  iShaderAttribute *m_attrLightDirectionPSSM;
  iShaderAttribute *m_attrDisancesPSSM;
  iShaderAttribute *m_attrShadowMatsProjView;
  iShaderAttribute *m_attrShadowMatsProj;
  iShaderAttribute *m_attrShadowMatsView;
  iShaderAttribute *m_attrShadowMap;
  iShaderAttribute *m_attrShadowColorMap;
  iShaderAttribute *m_attrShadowMapSizeInv;
  iShaderAttribute *m_attrMapBias;
  iShaderAttribute *m_attrShadowIntensity;
  iShaderAttribute *m_attrShadowProjNearFar;
  iTexture2DArray *m_colorBuffer;
  iTexture2DArray *m_depthBuffer;
  iTexture2DArray *m_colorBufferBlur;
  void BindDirectionalLightPSSM(csDirectionalLight *directionalLight);

};
