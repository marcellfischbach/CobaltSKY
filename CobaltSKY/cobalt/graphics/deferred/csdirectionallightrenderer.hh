#pragma once

#include <cobalt/graphics/deferred/cslightrenderer.hh>

struct iTexture2D;
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
  void RenderShadowMap(const csDirectionalLight *light, csGBuffer *gbuffer);
  void BlurShadowMap();
  void CalcPSSMMatrices(const csDirectionalLight *light, const csCamera *camera);
  void CalcMatrix(const csVector3f &dir, const csVector3f  &camPos, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const;

  void CalcPSSMMatricesAlternative(const csDirectionalLight *light, const csCamera *camera);
  void CalcMatrixAlternative(const csVector3f &dir, const csVector3f  &camPos, csSize numPoints, csVector3f *points, csMatrix4f &cam, csMatrix4f &camInv, csVector3f &min, csVector3f &max) const;
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
  iShaderAttribute *m_attrShadowMap;
  iTexture2DArray *m_colorBuffer;
  iTexture2DArray *m_depthBuffer;
  iTexture2DArray *m_colorBufferBlur;
  void BindDirectionalLightPSSM(csDirectionalLight *directionalLight);

  struct {
	  iShader *shader;
	  iShaderAttribute *attrDepth;
	  iShaderAttribute *attrDistances;
	  iShaderAttribute *attrShadowMatsProjView;
	  iShaderAttribute *attrShadowColorMap;
	  iShaderAttribute *attrShadowMap;
    iTexture2D *shadowMap;
    iRenderTarget *shadowRenderTarget;
    iTexture2D *shadowMapPingPong;
    iRenderTarget *shadowRenderTargetPingPong;
  } m_shadowMapRenderer;

  struct {
    iShader *shader;
    iShaderAttribute *attrColor0;
  } m_shadowMapBlurHori;

  struct {
    iShader *shader;
    iShaderAttribute *attrColor0;
  } m_shadowMapBlurVert;

};
