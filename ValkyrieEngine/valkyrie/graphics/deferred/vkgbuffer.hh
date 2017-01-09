#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/graphics/deferred/vkgbuffer.refl.hh>

struct iGraphics;
struct iTexture2D;
struct iRenderTarget;
struct iSampler;

VK_INTERFACE()
class VKE_API vkGBuffer : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkGBuffer(iGraphics *renderer);
  virtual ~vkGBuffer();

  bool Resize(vkUInt16 width, vkUInt16 height);

  bool Bind(iGraphics *renderer);

  bool IsValid() const;

  VK_FORCEINLINE iTexture2D *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  VK_FORCEINLINE iTexture2D *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  VK_FORCEINLINE iTexture2D *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  VK_FORCEINLINE iTexture2D *GetSSSSpec()
  {
    return m_sssSpec;
  }

  VK_FORCEINLINE iTexture2D *GetDepth()
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
