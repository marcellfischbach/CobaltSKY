#pragma once

#include <Valkyrie/graphics/deferred/vklightrenderer.hh>


struct ITexture2DArray;
class vkPointLight;


class vkPointLightRenderer : public vkLightRenderer
{
public:
  vkPointLightRenderer(IGraphics *renderer);
  virtual ~vkPointLightRenderer();

  virtual void Render(vkEntity *root, vkCamera *camera, vkLight *light, vkGBuffer *gbuffer, IRenderTarget *target);

private:
  void RenderShadow(vkEntity *root, const vkPointLight *light);
  void CalcCubeMatrices(const vkPointLight *light);

  vkMatrix4f m_shadowCam[6];
  vkMatrix4f m_shadowProj[6];
  vkMatrix4f m_shadowProjView[6];
  vkVector2f m_shadowNearFar[6];

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