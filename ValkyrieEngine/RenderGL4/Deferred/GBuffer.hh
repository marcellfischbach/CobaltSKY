#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Core/Object.hh>
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

private:
  vkTexture2DGL4 *m_diffuseRoughness;
  vkTexture2DGL4 *m_normalLightMode;
  vkTexture2DGL4 *m_emissiveMetallic;
  vkTexture2DGL4 *m_sssSpec;
  vkTexture2DGL4 *m_depth;
  vkRenderTargetGL4 *m_renderTarget;
};