
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
  m_depth = vkQueryClass<vkTexture2DGL4>(renderer->CreateTexture2D(ePF_D24S8, width, height));

  vkSamplerGL4 *colorSampler = vkGBuffer::GetColorSampler(renderer);
  vkSamplerGL4 *depthSampler = vkGBuffer::GetDepthSampler(renderer);

  m_diffuseRoughness->SetSampler(colorSampler);
  m_normalLightMode->SetSampler(colorSampler);
  m_emissiveMetallic->SetSampler(colorSampler);
  m_sssSpec->SetSampler(colorSampler);
  m_depth->SetSampler(depthSampler);


  m_renderTarget = vkQueryClass<vkRenderTargetGL4>(renderer->CreateRenderTarget());
  m_renderTarget->Initialize(width, height);
  m_renderTarget->AddColorTexture(m_diffuseRoughness);
  m_renderTarget->AddColorTexture(m_normalLightMode);
  m_renderTarget->AddColorTexture(m_emissiveMetallic);
  m_renderTarget->AddColorTexture(m_sssSpec);
  m_renderTarget->SetDepthTexture(m_depth);
  if (m_renderTarget->Finilize())
  {
    printf("Successfully created GBuffer\n");
  }
  else
  {
    VK_RELEASE(m_diffuseRoughness);
    VK_RELEASE(m_normalLightMode);
    VK_RELEASE(m_emissiveMetallic);
    VK_RELEASE(m_sssSpec);
    VK_RELEASE(m_renderTarget);
    printf("Failed to create GBuffer\n");
  }
}

bool vkGBuffer::IsValid() const
{
  return m_renderTarget != 0;
}

bool vkGBuffer::Bind(RendererGL4 *renderer)
{
  if (!m_renderTarget)
  {
    return false;
  }

  renderer->SetRenderTarget(m_renderTarget);
  renderer->SetViewport(m_renderTarget);

  return true;
}



vkSamplerGL4 *vkGBuffer::GetColorSampler(RendererGL4 *renderer)
{
  static vkSamplerGL4 *sampler = 0;
  if (!sampler)
  {
    sampler = static_cast<vkSamplerGL4*>(renderer->CreateSampler());
    sampler->SetFilter(eFM_MinMagNearest);
    sampler->SetAddressU(eTAM_ClampBorder);
    sampler->SetAddressV(eTAM_ClampBorder);
    sampler->SetAddressW(eTAM_ClampBorder);
  }

  return sampler;
}


vkSamplerGL4 *vkGBuffer::GetDepthSampler(RendererGL4 *renderer)
{
  static vkSamplerGL4 *sampler = 0;
  if (!sampler)
  {
    sampler = static_cast<vkSamplerGL4*>(renderer->CreateSampler());
    sampler->SetFilter(eFM_MinMagNearest);
    sampler->SetAddressU(eTAM_ClampBorder);
    sampler->SetAddressV(eTAM_ClampBorder);
    sampler->SetAddressW(eTAM_ClampBorder);
    sampler->SetTextureCompareMode(eTCM_None);
    sampler->SetTextureCompareFunc(eTCF_Less);
  }

  return sampler;
}
