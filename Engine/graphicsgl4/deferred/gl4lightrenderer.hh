#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/core/cscollection.hh>
#include <cobalt/csenums.hh>


namespace cs
{
struct iGraphics;
struct iRenderTarget;
struct iShaderAttribute;
class Camera;
class Clipper;
class Entity;
class GBufferGL4;
class Light;
class RenderState;
class ResourceLocator;
class SamplerWrapper;
class ShaderWrapper;


class LightRendererGL4
{
public:
  LightRendererGL4(cs::iGraphics* renderer);
  virtual ~LightRendererGL4();

  virtual void Render(cs::Entity* node, cs::Camera* camera, cs::Light* light, cs::GBufferGL4* gbuffer, cs::iRenderTarget* target) = 0;

  cs::iRenderTarget* GetShadowBuffer();

protected:

  struct GBufferAttribs
  {
    cs::iShaderAttribute* attrDiffuseRoughness;
    cs::iShaderAttribute* attrNormalLightMode;
    cs::iShaderAttribute* attrEmissiveMetallic;
    cs::iShaderAttribute* attrSSSSpecular;
    cs::iShaderAttribute* attrDepth;
  };

  struct LightProgram
  {
    cs::ShaderWrapper* program;
    GBufferAttribs gbuffer;
    cs::iShaderAttribute* attrColor;
    cs::iShaderAttribute* attrEnergy;
  };

  void InitializeLightProgram(LightProgram* lightProgram, const cs::ResourceLocator& locator);


  void BindGBuffer(GBufferAttribs& attribs, cs::GBufferGL4* gbuffer);
  void BindLight(LightProgram& lightProgram, cs::Light* light);

protected:
  void CalcShadowIntensity(const cs::Light* light);
  cs::Collection<cs::RenderState*> m_renderStates[cs::eRQ_COUNT];
  cs::iGraphics* m_renderer;
  float m_mapBias;

  // shadow buffer
  csUInt16 m_shadowBufferSize;
  cs::iRenderTarget* m_shadowBuffer;
  cs::Vector2f m_shadowIntensity;

  cs::SamplerWrapper* m_depthSampler;
};


}

CS_FORCEINLINE cs::iRenderTarget *cs::LightRendererGL4::GetShadowBuffer()
{
  return m_shadowBuffer;
}
