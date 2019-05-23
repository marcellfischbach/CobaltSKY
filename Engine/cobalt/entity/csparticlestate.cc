
#include <cobalt/entity/csparticlestate.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csparticle.hh>
#include <cobalt/entity/ientityscan.hh>
#include <math.h>

cs::ParticleState::ParticleState()
  : cs::RenderState()
  , m_particle(0)
  , m_materialInstance(0)
  , m_shadingMode(cs::ePSM_Emitting)
  , m_emitter(0)
  , m_stepper(0)
{
}

cs::ParticleState::~ParticleState()
{
  CS_RELEASE(m_particle);
  CS_RELEASE(m_materialInstance);
  CS_RELEASE(m_emitter);
  CS_RELEASE(m_stepper);
}

void cs::ParticleState::SetParticle(cs::Particle *particle)
{
  CS_SET(m_particle, particle);
}

cs::Particle *cs::ParticleState::GetParticle()
{
  return m_particle;
}

const cs::Particle *cs::ParticleState::GetParticle() const
{
  return m_particle;
}


void cs::ParticleState::SetMaterial(cs::Material *material)
{
  CS_SET(m_materialInstance, material);
}

cs::Material *cs::ParticleState::GetMaterial()
{
  return m_materialInstance;
}

const cs::Material *cs::ParticleState::GetMaterial() const
{
  return m_materialInstance;
}

void cs::ParticleState::SetShadingMode(cs::eParticleShadingMode shadingMode)
{
  m_shadingMode = shadingMode;
}

cs::eParticleShadingMode cs::ParticleState::GetShadingMode() const
{
  return m_shadingMode;
}

void cs::ParticleState::SetEmitter(cs::iParticleEmitter *emitter)
{
  CS_SET(m_emitter, emitter);
}

cs::iParticleEmitter *cs::ParticleState::GetEmitter()
{
  return m_emitter;
}

const cs::iParticleEmitter *cs::ParticleState::GetEmitter() const
{
  return m_emitter;
}

void cs::ParticleState::SetStepper(cs::iParticleStepper *stepper)
{
  CS_SET(m_stepper,  stepper);
}

cs::iParticleStepper *cs::ParticleState::GetStepper()
{
  return m_stepper;
}

const cs::iParticleStepper *cs::ParticleState::GetStepper() const
{
  return m_stepper;
}


void cs::ParticleState::Render(cs::iGraphics *graphics, cs::eRenderPass pass)  const
{
  cs::RenderState::Render(graphics, pass);
  if (m_particle && m_materialInstance)
  {
    switch (m_shadingMode)
    {
    case cs::ePSM_Emitting:
      graphics->SetBlendMode(cs::eBM_SrcAlpha, cs::eBM_One);
      break;
    case cs::ePSM_Shaded:
      graphics->SetBlendMode(cs::eBM_SrcAlpha, cs::eBM_InvSrcAlpha);
      break;
    }

    m_particle->Render(graphics, pass, m_materialInstance);
  }
}

void cs::ParticleState::PrivScan(cs::Clipper *clipper, cs::iGraphics *graphics, cs::iEntityScan *entityScan, const cs::ScanConfig &config)
{
  if (m_materialInstance && m_particle)
  {
    if (m_castShadow && config.ScanShadowCasters || !m_castShadow && config.ScanNonShadowCasters)
    {
      entityScan->ScanRenderState(this);
    }
  }
}


void cs::ParticleState::Update(float tpf)
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



