
#include <graphicsgl4/deferred/gl4gbuffer.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/irendertarget.hh>
#include <cobalt/graphics/isampler.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <stdio.h>


cs::GBufferGL4::GBufferGL4(cs::iGraphics *renderer)
  : cs::Object()
  , m_renderer(renderer)
  , m_diffuseRoughness(0)
  , m_normalLightMode(0)
  , m_emissiveMetallic(0)
  , m_sssSpec(0)
  , m_depth(0)
  , m_renderTarget(0)
{
}

cs::GBufferGL4::~GBufferGL4()
{
  CS_RELEASE(m_diffuseRoughness);
  CS_RELEASE(m_normalLightMode);
  CS_RELEASE(m_emissiveMetallic);
  CS_RELEASE(m_sssSpec);
  CS_RELEASE(m_depth);
  CS_RELEASE(m_renderTarget);
}

bool cs::GBufferGL4::Resize(csUInt16 width, csUInt16 height)
{
  CS_RELEASE(m_diffuseRoughness);
  CS_RELEASE(m_normalLightMode);
  CS_RELEASE(m_emissiveMetallic);
  CS_RELEASE(m_sssSpec);
  CS_RELEASE(m_depth);
  CS_RELEASE(m_renderTarget);

  m_diffuseRoughness = new cs::Texture2DWrapper(cs::QueryClass<cs::iTexture2D>(m_renderer->CreateTexture2D(cs::ePF_RGBA, width, height, false)));
  m_normalLightMode = new cs::Texture2DWrapper(cs::QueryClass<cs::iTexture2D>(m_renderer->CreateTexture2D(cs::ePF_R10G10B10A2, width, height, false)));
  m_emissiveMetallic = new cs::Texture2DWrapper(cs::QueryClass<cs::iTexture2D>(m_renderer->CreateTexture2D(cs::ePF_RGBA, width, height, false)));
  m_sssSpec = new cs::Texture2DWrapper(cs::QueryClass<cs::iTexture2D>(m_renderer->CreateTexture2D(cs::ePF_RGBA, width, height, false)));
  m_depth = new cs::Texture2DWrapper(cs::QueryClass<cs::iTexture2D>(m_renderer->CreateTexture2D(cs::ePF_D24S8, width, height, false)));

  cs::SamplerWrapper *colorSampler = cs::GBufferGL4::GetColorSampler(m_renderer);
  cs::SamplerWrapper *depthSampler = cs::GBufferGL4::GetDepthSampler(m_renderer);

  m_diffuseRoughness->Get()->SetSampler(colorSampler);
  m_normalLightMode->Get()->SetSampler(colorSampler);
  m_emissiveMetallic->Get()->SetSampler(colorSampler);
  m_sssSpec->Get()->SetSampler(colorSampler);
  m_depth->Get()->SetSampler(depthSampler);


  m_renderTarget = cs::QueryClass<cs::iRenderTarget>(m_renderer->CreateRenderTarget());
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

bool cs::GBufferGL4::IsValid() const
{
  return m_renderTarget != 0;
}

bool cs::GBufferGL4::Bind(cs::iGraphics *renderer)
{
  if (!m_renderTarget)
  {
    return false;
  }

  renderer->SetRenderTarget(m_renderTarget);
  renderer->SetViewport(m_renderTarget);

  return true;
}



cs::SamplerWrapper *cs::GBufferGL4::GetColorSampler(cs::iGraphics *renderer)
{
  static cs::SamplerWrapper *sampler = 0;
  if (!sampler)
  {
    cs::iSampler *smplr = renderer->CreateSampler();
    smplr->SetFilter(cs::eFM_MinMagNearest);
    smplr->SetAddressU(cs::eTAM_ClampBorder);
    smplr->SetAddressV(cs::eTAM_ClampBorder);
    smplr->SetAddressW(cs::eTAM_ClampBorder);
    sampler = new cs::SamplerWrapper(smplr);
  }

  return sampler;
}


cs::SamplerWrapper *cs::GBufferGL4::GetDepthSampler(cs::iGraphics *renderer)
{
  static cs::SamplerWrapper *sampler = 0;
  if (!sampler)
  {
    cs::iSampler *smplr = renderer->CreateSampler();
    smplr->SetFilter(cs::eFM_MinMagNearest);
    smplr->SetAddressU(cs::eTAM_ClampBorder);
    smplr->SetAddressV(cs::eTAM_ClampBorder);
    smplr->SetAddressW(cs::eTAM_ClampBorder);
    smplr->SetTextureCompareMode(cs::eTCM_None);
    smplr->SetTextureCompareFunc(cs::eTCF_Less);
    sampler = new cs::SamplerWrapper(smplr);
  }

  return sampler;
}
