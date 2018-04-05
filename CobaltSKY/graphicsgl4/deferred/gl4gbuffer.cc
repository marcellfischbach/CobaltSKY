
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <cobalt/graphics/isampler.hh>
#include <stdio.h>


csGBufferGL4::csGBufferGL4(iGraphics *renderer)
  : csObject ()
  , m_renderer(renderer)
  , m_diffuseRoughness(0)
  , m_normalLightMode(0)
  , m_emissiveMetallic(0)
  , m_sssSpec(0)
  , m_depth(0)
  , m_renderTarget(0)
{
}

csGBufferGL4::~csGBufferGL4()
{
  CS_RELEASE(m_diffuseRoughness);
  CS_RELEASE(m_normalLightMode);
  CS_RELEASE(m_emissiveMetallic);
  CS_RELEASE(m_sssSpec);
  CS_RELEASE(m_depth);
  CS_RELEASE(m_renderTarget);
}

bool csGBufferGL4::Resize(csUInt16 width, csUInt16 height)
{
  CS_RELEASE(m_diffuseRoughness);
  CS_RELEASE(m_normalLightMode);
  CS_RELEASE(m_emissiveMetallic);
  CS_RELEASE(m_sssSpec);
  CS_RELEASE(m_depth);
  CS_RELEASE(m_renderTarget);

  m_diffuseRoughness = csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_normalLightMode = csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_R10G10B10A2, width, height, false));
  m_emissiveMetallic = csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_sssSpec = csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false));
  m_depth = csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_D24S8, width, height, false));

  iSampler *colorSampler = csGBufferGL4::GetColorSampler(m_renderer);
  iSampler *depthSampler = csGBufferGL4::GetDepthSampler(m_renderer);

  m_diffuseRoughness->SetSampler(colorSampler);
  m_normalLightMode->SetSampler(colorSampler);
  m_emissiveMetallic->SetSampler(colorSampler);
  m_sssSpec->SetSampler(colorSampler);
  m_depth->SetSampler(depthSampler);


  m_renderTarget = csQueryClass<iRenderTarget>(m_renderer->CreateRenderTarget());
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
    CS_RELEASE(m_diffuseRoughness);
    CS_RELEASE(m_normalLightMode);
    CS_RELEASE(m_emissiveMetallic);
    CS_RELEASE(m_sssSpec);
    CS_RELEASE(m_renderTarget);
    printf("Failed to create GBuffer\n");
    return false;
  }
}

bool csGBufferGL4::IsValid() const
{
  return m_renderTarget != 0;
}

bool csGBufferGL4::Bind(iGraphics *renderer)
{
  if (!m_renderTarget)
  {
    return false;
  }

  renderer->SetRenderTarget(m_renderTarget);
  renderer->SetViewport(m_renderTarget);

  return true;
}



iSampler *csGBufferGL4::GetColorSampler(iGraphics *renderer)
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


iSampler *csGBufferGL4::GetDepthSampler(iGraphics *renderer)
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
