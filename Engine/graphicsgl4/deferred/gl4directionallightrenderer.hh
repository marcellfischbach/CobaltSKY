#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/deferred/gl4lightrenderer.hh>


namespace cs
{
class DirectionalLight;
class Texture2DWrapper;
class Texture2DArrayWrapper;

class DirectionalLightRendererGL4 : public cs::LightRendererGL4
{
public:
  DirectionalLightRendererGL4(cs::iGraphics *renderer);
  virtual ~DirectionalLightRendererGL4();

  virtual void Render(cs::Entity *root, cs::Camera *camera, cs::Light *light, cs::GBufferGL4 *gbuffer, cs::iRenderTarget *target);

private:
  void RenderShadow(cs::Entity *root, cs::Camera *camera, const cs::DirectionalLight *light);
  void RenderShadowMap(const cs::DirectionalLight *light, cs::GBufferGL4 *gbuffer);
  void BlurShadowMap();
  void CalcPSSMMatrices(const cs::DirectionalLight *light, const cs::Camera *camera);
  void CalcMatrix(const cs::Vector3f &dir, const cs::Vector3f  &camPos, csSize numPoints, cs::Vector3f *points, cs::Matrix4f &cam, cs::Matrix4f &camInv, cs::Vector3f &min, cs::Vector3f &max) const;

  void CalcPSSMMatricesAlternative(const cs::DirectionalLight *light, const cs::Camera *camera);
  void CalcMatrixAlternative(const cs::Vector3f &dir, const cs::Vector3f  &camPos, csSize numPoints, cs::Vector3f *points, cs::Matrix4f &cam, cs::Matrix4f &camInv, cs::Vector3f &min, cs::Vector3f &max) const;
  void UpdateProjectionMatrices();
  cs::Clipper *CreateClipper();

  cs::Matrix4f m_shadowCam[3];
  cs::Matrix4f m_shadowCamInv[3];
  cs::Matrix4f m_shadowProj[3];
  cs::Matrix4f m_shadowProjView[3];
  cs::Vector2f m_shadowNearFar[3];
  cs::Vector3f m_min[3];
  cs::Vector3f m_max[3];

  cs::Matrix4f m_shadowCamAll;
  cs::Matrix4f m_shadowCamInvAll;
  cs::Vector3f m_minAll;
  cs::Vector3f m_maxAll;

  cs::Vector3f m_distances;

  LightProgram m_programNoShadow;
  cs::iShaderAttribute *m_attrLightDirectionNoShadow;
  void BindDirectionalLightNoShadow(cs::DirectionalLight *directionalLight);

  LightProgram m_programPSSM;
  cs::iShaderAttribute *m_attrLightDirectionPSSM;
  cs::iShaderAttribute *m_attrShadowMap;
  cs::Texture2DArrayWrapper *m_colorBuffer;
  cs::Texture2DArrayWrapper *m_depthBuffer;
  cs::Texture2DArrayWrapper *m_colorBufferBlur;
  void BindDirectionalLightPSSM(cs::DirectionalLight *directionalLight);

  struct {
    cs::ShaderWrapper *shader;
	  cs::iShaderAttribute *attrDepth;
	  cs::iShaderAttribute *attrDistances;
	  cs::iShaderAttribute *attrShadowMatsProjView;
	  cs::iShaderAttribute *attrShadowColorMap;
	  cs::iShaderAttribute *attrShadowMap;
   cs::Texture2DWrapper *shadowMap;
    cs::iRenderTarget *shadowRenderTarget;
    cs::Texture2DWrapper *shadowMapPingPong;
    cs::iRenderTarget *shadowRenderTargetPingPong;
  } m_shadowMapRenderer;

  struct {
    cs::ShaderWrapper *shader;
    cs::iShaderAttribute *attrColor0;
  } m_shadowMapBlurHori;

  struct {
    cs::ShaderWrapper *shader;
    cs::iShaderAttribute *attrColor0;
  } m_shadowMapBlurVert;

};

}
