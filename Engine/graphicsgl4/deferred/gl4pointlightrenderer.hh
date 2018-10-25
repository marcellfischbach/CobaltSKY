#pragma once

#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/deferred/gl4lightrenderer.hh>


struct csTexture2DArrayWrapper;
class csPointLight;


class csPointLightRendererGL4 : public csLightRendererGL4
{
public:
  csPointLightRendererGL4(iGraphics *renderer);
  virtual ~csPointLightRendererGL4();

  virtual void Render(csEntity *root, csCamera *camera, csLight *light, csGBufferGL4 *gbuffer, iRenderTarget *target);

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
  csTexture2DArrayWrapper *m_depthBuffer;
  void BindPointLightCubeShadow(csPointLight *pointLight);

};