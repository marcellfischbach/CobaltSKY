
#include <valkyrie/entity/vkparticlestate.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/vkmaterial.hh>
#include <valkyrie/graphics/vkparticle.hh>
#include <valkyrie/entity/ientityscan.hh>
#include <math.h>

vkParticleState::vkParticleState()
  : vkRenderState()
  , m_particle(0)
  , m_materialInstance(0)
  , m_shadingMode(ePSM_Emitting)
  , m_emitter(0)
  , m_stepper(0)
{
}

vkParticleState::~vkParticleState()
{
  VK_RELEASE(m_particle);
  VK_RELEASE(m_materialInstance);
  VK_RELEASE(m_emitter);
  VK_RELEASE(m_stepper);
}

void vkParticleState::SetParticle(vkParticle *particle)
{
  VK_SET(m_particle, particle);
}

vkParticle *vkParticleState::GetParticle()
{
  return m_particle;
}

const vkParticle *vkParticleState::GetParticle() const
{
  return m_particle;
}


void vkParticleState::SetMaterial(vkMaterial *material)
{
  VK_SET(m_materialInstance, material);
}

vkMaterial *vkParticleState::GetMaterial()
{
  return m_materialInstance;
}

const vkMaterial *vkParticleState::GetMaterial() const
{
  return m_materialInstance;
}

void vkParticleState::SetShadingMode(vkParticleShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

vkParticleShadingMode vkParticleState::GetShadingMode() const
{
  return m_shadingMode;
}

void vkParticleState::SetEmitter(iParticleEmitter *emitter)
{
  VK_SET(m_emitter, emitter);
}

iParticleEmitter *vkParticleState::GetEmitter()
{
  return m_emitter;
}

const iParticleEmitter *vkParticleState::GetEmitter() const
{
  return m_emitter;
}

void vkParticleState::SetStepper(iParticleStepper *stepper)
{
  VK_SET(m_stepper,  stepper);
}

iParticleStepper *vkParticleState::GetStepper()
{
  return m_stepper;
}

const iParticleStepper *vkParticleState::GetStepper() const
{
  return m_stepper;
}


void vkParticleState::Render(iGraphics *graphics, vkRenderPass pass)  const
{
  vkRenderState::Render(graphics, pass);
  if (m_particle && m_materialInstance)
  {
    switch (m_shadingMode)
    {
    case ePSM_Emitting:
      graphics->SetBlendMode(eBM_SrcAlpha, eBM_One);
      break;
    case ePSM_Shaded:
      graphics->SetBlendMode(eBM_SrcAlpha, eBM_InvSrcAlpha);
      break;
    }

    m_particle->Render(graphics, pass, m_materialInstance);
  }
}

void vkParticleState::PrivScan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config)
{
  if (m_materialInstance && m_particle)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      entityScan->ScanRenderState(this);
    }
  }
}


void vkParticleState::Update(float tpf)
{
  if (m_particle)
  {
    if (m_stepper)
    {
      m_stepper->Update(tpf, m_particle);
    }
    if (m_emitter)
    {
      m_emitter->Update(tpf, m_particle);
    }
  }
}



