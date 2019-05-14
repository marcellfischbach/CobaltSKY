#pragma once

#include <graphicsgl4/gl4export.hh>
#include <csrefl/class.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/cstypes.hh>
#include <graphicsgl4/deferred/gl4gbuffer.refl.hh>

struct iGraphics;
struct iRenderTarget;
class csTexture2DWrapper;
class csSamplerWrapper;

CS_CLASS()
class CSGRAPHICSGL4_API csGBufferGL4 : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;
public:
  csGBufferGL4(iGraphics *renderer);
  virtual ~csGBufferGL4();

  bool Resize(csUInt16 width, csUInt16 height);

  bool Bind(iGraphics *renderer);

  bool IsValid() const;

  CS_FORCEINLINE csTexture2DWrapper *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  CS_FORCEINLINE csTexture2DWrapper *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  CS_FORCEINLINE csTexture2DWrapper *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  CS_FORCEINLINE csTexture2DWrapper *GetSSSSpec()
  {
    return m_sssSpec;
  }

  CS_FORCEINLINE csTexture2DWrapper *GetDepth()
  {
    return m_depth;
  }

  static csSamplerWrapper *GetColorSampler(iGraphics *renderer);
  static csSamplerWrapper *GetDepthSampler(iGraphics *renderer);

private:
  csTexture2DWrapper *m_diffuseRoughness;
  csTexture2DWrapper *m_normalLightMode;
  csTexture2DWrapper *m_emissiveMetallic;
  csTexture2DWrapper *m_sssSpec;
  csTexture2DWrapper *m_depth;
  iRenderTarget *m_renderTarget;
  iGraphics *m_renderer;
};
