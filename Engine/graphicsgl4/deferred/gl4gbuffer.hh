#pragma once

#include <graphicsgl4/gl4export.hh>
#include <csrefl/class.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <graphicsgl4/deferred/gl4gbuffer.refl.hh>


namespace cs
{
struct iGraphics;
struct iRenderTarget;
class SamplerWrapper;
class Texture2DWrapper;
}

CS_CLASS()
class CSGRAPHICSGL4_API csGBufferGL4 : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  csGBufferGL4(cs::iGraphics *renderer);
  virtual ~csGBufferGL4();

  bool Resize(csUInt16 width, csUInt16 height);

  bool Bind(cs::iGraphics *renderer);

  bool IsValid() const;

  CS_FORCEINLINE cs::Texture2DWrapper *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  CS_FORCEINLINE cs::Texture2DWrapper *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  CS_FORCEINLINE cs::Texture2DWrapper *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  CS_FORCEINLINE cs::Texture2DWrapper *GetSSSSpec()
  {
    return m_sssSpec;
  }

  CS_FORCEINLINE cs::Texture2DWrapper *GetDepth()
  {
    return m_depth;
  }

  static cs::SamplerWrapper *GetColorSampler(cs::iGraphics *renderer);
  static cs::SamplerWrapper *GetDepthSampler(cs::iGraphics *renderer);

private:
  cs::Texture2DWrapper *m_diffuseRoughness;
  cs::Texture2DWrapper *m_normalLightMode;
  cs::Texture2DWrapper *m_emissiveMetallic;
  cs::Texture2DWrapper *m_sssSpec;
  cs::Texture2DWrapper *m_depth;
  cs::iRenderTarget *m_renderTarget;
  cs::iGraphics *m_renderer;
};
