#pragma once

#include <valkyrie/graphics/deferred/vklightrenderer.hh>


struct iTexture2DArray;
class vkPointLight;


class vkPointLightRenderer : public vkLightRenderer
{
public:
  vkPointLightRenderer(iGraphics *renderer);
  virtual ~vkPointLightRenderer();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, iRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, const vkPointLight *light);
  void CalcCubeMatrices(const vkPointLight *light);

  vkMatrix4f m_shadowCam[6];
  vkMatrix4f m_shadowProj[6];
  vkMatrix4f m_shadowProjView[6];
  vkVector2f m_shadowNearFar[6];

  LightProgram m_programNoShadow;
  iShaderAttribute *m_attrLightPositionNoShadow;
  iShaderAttribute *m_attrLightRangeNoShadow;
  void BindPointLightNo(vkPointLight *pointLight);


  LightProgram m_programCubeShadow;
  iShaderAttribute *m_attrLightPositionCubeShadow;
  iShaderAttribute *m_attrLightRangeCubeShadow;
  iShaderAttribute *m_attrShadowMats;
  iShaderAttribute *m_attrShadowMap;
  iShaderAttribute *m_attrMapBias;
  iShaderAttribute *m_attrShadowIntensity;
  iTexture2DArray *m_depthBuffer;
  void BindPointLightCubeShadow(vkPointLight *pointLight);

};