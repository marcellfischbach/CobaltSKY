#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csdefs.hh>
#include <graphicsgl4/deferred/gl4gbuffer.refl.hh>

struct iGraphics;
struct iTexture2D;
struct iRenderTarget;
struct iSampler;

CS_INTERFACE()
class CSGRAPHICSGL4_API csGBufferGL4 : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;
public:
  csGBufferGL4(iGraphics *renderer);
  virtual ~csGBufferGL4();

  bool Resize(csUInt16 width, csUInt16 height);

  bool Bind(iGraphics *renderer);

  bool IsValid() const;

  CS_FORCEINLINE iTexture2D *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  CS_FORCEINLINE iTexture2D *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  CS_FORCEINLINE iTexture2D *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  CS_FORCEINLINE iTexture2D *GetSSSSpec()
  {
    return m_sssSpec;
  }

  CS_FORCEINLINE iTexture2D *GetDepth()
  {
    return m_depth;
  }

  static iSampler *GetColorSampler(iGraphics *renderer);
  static iSampler *GetDepthSampler(iGraphics *renderer);

private:
  iTexture2D *m_diffuseRoughness;
  iTexture2D *m_normalLightMode;
  iTexture2D *m_emissiveMetallic;
  iTexture2D *m_sssSpec;
  iTexture2D *m_depth;
  iRenderTarget *m_renderTarget;
  iGraphics *m_renderer;
};
