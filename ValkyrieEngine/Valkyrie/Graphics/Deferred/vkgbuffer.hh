#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkdefs.hh>
#include <Valkyrie/Graphics/Deferred/GBuffer.refl.hh>

struct IGraphics;
struct ITexture2D;
struct IRenderTarget;
struct ISampler;

VK_INTERFACE()
class VKE_API vkGBuffer : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;
public:
  vkGBuffer(IGraphics *renderer);
  virtual ~vkGBuffer();

  bool Resize(vkUInt16 width, vkUInt16 height);

  bool Bind(IGraphics *renderer);

  bool IsValid() const;

  VK_FORCEINLINE ITexture2D *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  VK_FORCEINLINE ITexture2D *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  VK_FORCEINLINE ITexture2D *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  VK_FORCEINLINE ITexture2D *GetSSSSpec()
  {
    return m_sssSpec;
  }

  VK_FORCEINLINE ITexture2D *GetDepth()
  {
    return m_depth;
  }

  static ISampler *GetColorSampler(IGraphics *renderer);
  static ISampler *GetDepthSampler(IGraphics *renderer);

private:
  ITexture2D *m_diffuseRoughness;
  ITexture2D *m_normalLightMode;
  ITexture2D *m_emissiveMetallic;
  ITexture2D *m_sssSpec;
  ITexture2D *m_depth;
  IRenderTarget *m_renderTarget;
  IGraphics *m_renderer;
};
