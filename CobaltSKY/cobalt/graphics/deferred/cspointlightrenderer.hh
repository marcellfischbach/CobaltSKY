#pragma once

#include <cobalt/graphics/deferred/cslightrenderer.hh>


struct iTexture2DArray;
class csPointLight;


class csPointLightRenderer : public csLightRenderer
{
public:
  csPointLightRenderer(iGraphics *renderer);
  virtual ~csPointLightRenderer();

  virtual void Render(csEntity *root, csCamera *camera, csLight *light, csGBuffer *gbuffer, iRenderTarget *target);

private:
  void RenderShadow(csEntity *root, const csPointLight *light);
  void CalcCubeMatrices(const csPointLight *light);

  csMatrix4f m_shadowCam[6];
  csMatrix4f m_shadowProj[6];
  csMatrix4f m_shadowProjView[6];
  csVector2f m_shadowNearFar[6];

  LightProgram m_programNoShadow;
  iShaderAttribute *m_attrLightPositionNoShadow;
  iShaderAttribute *m_attrLightRangeNoShadow;
  void BindPointLightNo(csPointLight *pointLight);


  LightProgram m_programCubeShadow;
  iShaderAttribute *m_attrLightPositionCubeShadow;
  iShaderAttribute *m_attrLightRangeCubeShadow;
  iShaderAttribute *m_attrShadowMats;
  iShaderAttribute *m_attrShadowMap;
  iShaderAttribute *m_attrMapBias;
  iShaderAttribute *m_attrShadowIntensity;
  iTexture2DArray *m_depthBuffer;
  void BindPointLightCubeShadow(csPointLight *pointLight);

};