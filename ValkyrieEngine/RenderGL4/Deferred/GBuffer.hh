#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/Deferred/GBuffer.refl.hh>

class RendererGL4;
class vkTexture2DGL4;
class vkRenderTargetGL4;

VK_INTERFACE();
class VKGL4_API vkGBuffer : public vkObject
{
  VK_CLASS_GEN;
public:
  vkGBuffer(RendererGL4 *renderer, vkUInt16 width, vkUInt16 height);

  bool Bind(RendererGL4 *renderer);

  bool IsValid() const;

  VK_FORCEINLINE vkTexture2DGL4 *GetDiffuseRoughness()
  {
    return m_diffuseRoughness;
  }

  VK_FORCEINLINE vkTexture2DGL4 *GetNormalLightMode()
  {
    return m_normalLightMode;
  }

  VK_FORCEINLINE vkTexture2DGL4 *GetEmissiveMetallic()
  {
    return m_emissiveMetallic;
  }

  VK_FORCEINLINE vkTexture2DGL4 *GetSSSSpec()
  {
    return m_sssSpec;
  }

  VK_FORCEINLINE vkTexture2DGL4 *GetDepth()
  {
    return m_depth;
  }

private:
  vkTexture2DGL4 *m_diffuseRoughness;
  vkTexture2DGL4 *m_normalLightMode;
  vkTexture2DGL4 *m_emissiveMetallic;
  vkTexture2DGL4 *m_sssSpec;
  vkTexture2DGL4 *m_depth;
  vkRenderTargetGL4 *m_renderTarget;

  static vkSamplerGL4 *GetColorSampler(RendererGL4 *renderer);
  static vkSamplerGL4 *GetDepthSampler(RendererGL4 *renderer);
};