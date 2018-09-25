
#include <cobalt/entity/csparticlestate.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csparticle.hh>
#include <cobalt/entity/ientityscan.hh>
#include <math.h>

csParticleState::csParticleState()
  : csRenderState()
  , m_particle(0)
  , m_materialInstance(0)
  , m_shadingMode(ePSM_Emitting)
  , m_emitter(0)
  , m_stepper(0)
{
}

csParticleState::~csParticleState()
{
  CS_RELEASE(m_particle);
  CS_RELEASE(m_materialInstance);
  CS_RELEASE(m_emitter);
  CS_RELEASE(m_stepper);
}

void csParticleState::SetParticle(csParticle *particle)
{
  CS_SET(m_particle, particle);
}

csParticle *csParticleState::GetParticle()
{
  return m_particle;
}

const csParticle *csParticleState::GetParticle() const
{
  return m_particle;
}


void csParticleState::SetMaterial(csMaterial *material)
{
  CS_SET(m_materialInstance, material);
}

csMaterial *csParticleState::GetMaterial()
{
  return m_materialInstance;
}

const csMaterial *csParticleState::GetMaterial() const
{
  return m_materialInstance;
}

void csParticleState::SetShadingMode(csParticleShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

csParticleShadingMode csParticleState::GetShadingMode() const
{
  return m_shadingMode;
}

void csParticleState::SetEmitter(iParticleEmitter *emitter)
{
  CS_SET(m_emitter, emitter);
}

iParticleEmitter *csParticleState::GetEmitter()
{
  return m_emitter;
}

const iParticleEmitter *csParticleState::GetEmitter() const
{
  return m_emitter;
}

void csParticleState::SetStepper(iParticleStepper *stepper)
{
  CS_SET(m_stepper,  stepper);
}

iParticleStepper *csParticleState::GetStepper()
{
  return m_stepper;
}

const iParticleStepper *csParticleState::GetStepper() const
{
  return m_stepper;
}


void csParticleState::Render(iGraphics *graphics, csRenderPass pass)  const
{
  csRenderState::Render(graphics, pass);
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

void csParticleState::PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config)
{
  if (m_materialInstance && m_particle)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      entityScan->ScanRenderState(this);
    }
  }
}


void csParticleState::Update(float tpf)
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



