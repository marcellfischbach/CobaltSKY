#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/deferred/gl4lightrenderer.hh>


namespace cs
{
class PointLight;
class Texture2DArrayWrapper;
}


class csPointLightRendererGL4 : public csLightRendererGL4
{
public:
  csPointLightRendererGL4(cs::iGraphics *renderer);
  virtual ~csPointLightRendererGL4();

  virtual void Render(cs::Entity *root, cs::Camera *camera, cs::Light *light, csGBufferGL4 *gbuffer, cs::iRenderTarget *target);

private:
  void RenderShadow(cs::Entity *root, const cs::PointLight *light);
  void CalcCubeMatrices(const cs::PointLight *light);

  cs::Matrix4f m_shadowCam[6];
  cs::Matrix4f m_shadowProj[6];
  cs::Matrix4f m_shadowProjView[6];
  cs::Vector2f m_shadowNearFar[6];

  LightProgram m_programNoShadow;
  cs::iShaderAttribute *m_attrLightPositionNoShadow;
  cs::iShaderAttribute *m_attrLightRangeNoShadow;
  void BindPointLightNo(cs::PointLight *pointLight);


  LightProgram m_programCubeShadow;
  cs::iShaderAttribute *m_attrLightPositionCubeShadow;
  cs::iShaderAttribute *m_attrLightRangeCubeShadow;
  cs::iShaderAttribute *m_attrShadowMats;
  cs::iShaderAttribute *m_attrShadowMap;
  cs::iShaderAttribute *m_attrMapBias;
  cs::iShaderAttribute *m_attrShadowIntensity;
  cs::Texture2DArrayWrapper *m_depthBuffer;
  void BindPointLightCubeShadow(cs::PointLight *pointLight);

};