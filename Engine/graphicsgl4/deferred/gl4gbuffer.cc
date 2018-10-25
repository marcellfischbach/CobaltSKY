
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/cstexturewrapper.hh>
#include <cobalt/graphics/cssamplerwrapper.hh>
#include <stdio.h>


csGBufferGL4::csGBufferGL4(iGraphics *renderer)
  : csObject()
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

  m_diffuseRoughness = new csTexture2DWrapper(csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false)));
  m_normalLightMode = new csTexture2DWrapper(csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_R10G10B10A2, width, height, false)));
  m_emissiveMetallic = new csTexture2DWrapper(csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false)));
  m_sssSpec = new csTexture2DWrapper(csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_RGBA, width, height, false)));
  m_depth = new csTexture2DWrapper(csQueryClass<iTexture2D>(m_renderer->CreateTexture2D(ePF_D24S8, width, height, false)));

  csSamplerWrapper *colorSampler = csGBufferGL4::GetColorSampler(m_renderer);
  csSamplerWrapper *depthSampler = csGBufferGL4::GetDepthSampler(m_renderer);

  m_diffuseRoughness->Get()->SetSampler(colorSampler);
  m_normalLightMode->Get()->SetSampler(colorSampler);
  m_emissiveMetallic->Get()->SetSampler(colorSampler);
  m_sssSpec->Get()->SetSampler(colorSampler);
  m_depth->Get()->SetSampler(depthSampler);


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



csSamplerWrapper *csGBufferGL4::GetColorSampler(iGraphics *renderer)
{
  static csSamplerWrapper *sampler = 0;
  if (!sampler)
  {
    iSampler *smplr = renderer->CreateSampler();
    smplr->SetFilter(eFM_MinMagNearest);
    smplr->SetAddressU(eTAM_ClampBorder);
    smplr->SetAddressV(eTAM_ClampBorder);
    smplr->SetAddressW(eTAM_ClampBorder);
    sampler = new csSamplerWrapper(smplr);
  }

  return sampler;
}


csSamplerWrapper *csGBufferGL4::GetDepthSampler(iGraphics *renderer)
{
  static csSamplerWrapper *sampler = 0;
  if (!sampler)
  {
    iSampler *smplr = renderer->CreateSampler();
    smplr->SetFilter(eFM_MinMagNearest);
    smplr->SetAddressU(eTAM_ClampBorder);
    smplr->SetAddressV(eTAM_ClampBorder);
    smplr->SetAddressW(eTAM_ClampBorder);
    smplr->SetTextureCompareMode(eTCM_None);
    smplr->SetTextureCompareFunc(eTCF_Less);
    sampler = new csSamplerWrapper(smplr);
  }

  return sampler;
}
