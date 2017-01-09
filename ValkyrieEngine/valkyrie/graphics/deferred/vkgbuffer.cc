
#include <valkyrie/graphics/deferred/vkgbuffer.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/irendertarget.hh>
#include <valkyrie/graphics/itexture2d.hh>
#include <valkyrie/graphics/isampler.hh>
#include <stdio.h>


vkGBuffer::vkGBuffer(iGraphics *renderer)
  : vkObject ()
  , m_renderer(renderer)
  , m_diffuseRoughness(0)
  , m_normalLightMode(0)
  , m_emissiveMetallic(0)
  , m_sssSpec(0)
  , m_depth(0)
  , m_renderTarget(0)
{
}

vkGBuffer::~vkGBuffer()
{
  VK_RELEASE(m_diffuseRoughness);
  VK_RELEASE(m_normalLightMode);
  VK_RELEASE(m_emissiveMetallic);
  VK_RELEASE(m_sssSpec);
  VK_RELEASE(m_depth);
  VK_RELEASE(m_renderTarget);
}

bool vkGBuffer::Resize(vkUInt16 width, vkUInt16 height)
{
  VK_RELEASE(m_diffuseRoughness);
  VK_RELEASE(m_normalLightMode);
  VK_RELEASE(m_emissiveMetallic);
  VK_RELEASE(m_sssSpec);
  VK_RELEASE(m_depth);
  VK_RELEASE(m_renderTarget);

  m_diffuseRoughness = vkQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_normalLightMode = vkQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_R10G10B10A2, width, height, false));
  m_emissiveMetallic = vkQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_sssSpec = vkQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_depth = vkQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_D24S8, width, height, false));

  iSampler *colorSampler = vkGBuffer::GetColorSampler(m_renderer);
  iSampler *depthSampler = vkGBuffer::GetDepthSampler(m_renderer);

  m_diffuseRoughness->SetSampler(colorSampler);
  m_normalLightMode->SetSampler(colorSampler);
  m_emissiveMetallic->SetSampler(colorSampler);
  m_sssSpec->SetSampler(colorSampler);
  m_depth->SetSampler(depthSampler);


  m_renderTarget = vkQueryClass<iRenderTarget>(m_renderer->CreateRenderTarget());
  m_renderTarget->Initialize(width, height);
  m_renderTarget->AddColorTexture(m_diffuseRoughness);
  m_renderTarget->AddColorTexture(m_normalLightMode);
  m_renderTarget->AddColorTexture(m_emissiveMetallic);
  m_renderTarget->AddColorTexture(m_sssSpec);
  m_renderTarget->SetDepthTexture(m_depth);
  if (m_renderTarget->Finilize())
  {
    return true;
  }
  else
  {
    VK_RELEASE(m_diffuseRoughness);
    VK_RELEASE(m_normalLightMode);
    VK_RELEASE(m_emissiveMetallic);
    VK_RELEASE(m_sssSpec);
    VK_RELEASE(m_renderTarget);
    printf("Failed to create GBuffer\n");
    return false;
  }
}

bool vkGBuffer::IsValid() const
{
  return m_renderTarget != 0;
}

bool vkGBuffer::Bind(iGraphics *renderer)
{
  if (!m_renderTarget)
  {
    return false;
  }

  renderer->SetRenderTarget(m_renderTarget);
  renderer->SetViewport(m_renderTarget);

  return true;
}



iSampler *vkGBuffer::GetColorSampler(iGraphics *renderer)
{
  static iSampler *sampler = 0;
  if (!sampler)
  {
    sampler = renderer->CreateSampler();
    sampler->SetFilter(eFM_MinMagNearest);
    sampler->SetAddressU(eTAM_ClampBorder);
    sampler->SetAddressV(eTAM_ClampBorder);
    sampler->SetAddressW(eTAM_ClampBorder);
  }

  return sampler;
}


iSampler *vkGBuffer::GetDepthSampler(iGraphics *renderer)
{
  static iSampler *sampler = 0;
  if (!sampler)
  {
    sampler = renderer->CreateSampler();
    sampler->SetFilter(eFM_MinMagNearest);
    sampler->SetAddressU(eTAM_ClampBorder);
    sampler->SetAddressV(eTAM_ClampBorder);
    sampler->SetAddressW(eTAM_ClampBorder);
    sampler->SetTextureCompareMode(eTCM_None);
    sampler->SetTextureCompareFunc(eTCF_Less);
  }

  return sampler;
}
