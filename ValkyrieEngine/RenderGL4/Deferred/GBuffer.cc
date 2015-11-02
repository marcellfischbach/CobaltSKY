
#include <RenderGL4/Deferred/GBuffer.hh>
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/RenderTargetGL4.hh>
#include <RenderGL4/TextureGL4.hh>
#include <stdio.h>


vkGBuffer::vkGBuffer(RendererGL4 *renderer, vkUInt16 width, vkUInt16 height)
{
  m_diffuseRoughness = vkQueryClass<vkTexture2DGL4>(renderer->CreateTexture2D(ePF_RGBA, width, height));
  m_normalLightMode = vkQueryClass<vkTexture2DGL4>(renderer->CreateTexture2D(ePF_R10G10B10A2, width, height));
  m_emissiveMetallic = vkQueryClass<vkTexture2DGL4>(renderer->CreateTexture2D(ePF_RGBA, width, height));
  m_sssSpec = vkQueryClass<vkTexture2DGL4>(renderer->CreateTexture2D(ePF_RGBA, width, height));


  m_renderTarget = vkQueryClass<vkRenderTargetGL4>(renderer->CreateRenderTarget());
  m_renderTarget->Initialize();
  m_renderTarget->AddColorTexture(m_diffuseRoughness);
  m_renderTarget->AddColorTexture(m_normalLightMode);
  m_renderTarget->AddColorTexture(m_emissiveMetallic);
  m_renderTarget->AddColorTexture(m_sssSpec);
  m_renderTarget->SetDepthBuffer(width, height);
  if (m_renderTarget->Finilize())
  {
    printf("Successfully created GBuffer\n");
  }
  else
  {
    printf("Failed to create GBuffer\n");
  }
}