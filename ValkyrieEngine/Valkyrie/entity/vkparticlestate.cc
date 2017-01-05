
#include <Valkyrie/entity/vkparticlestate.hh>
#include <Valkyrie/graphics/igraphics.hh>
#include <Valkyrie/graphics/ivertexbuffer.hh>
#include <Valkyrie/graphics/vkmaterialinstance.hh>
#include <Valkyrie/graphics/vkparticle.hh>
#include <Valkyrie/entity/ientityscan.hh>
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


void vkParticleState::SetMaterial(vkMaterialInstance *material)
{
  VK_SET(m_materialInstance, material);
}

vkMaterialInstance *vkParticleState::GetMaterial()
{
  return m_materialInstance;
}

const vkMaterialInstance *vkParticleState::GetMaterial() const
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

void vkParticleState::SetEmitter(IParticleEmitter *emitter)
{
  VK_SET(m_emitter, emitter);
}

IParticleEmitter *vkParticleState::GetEmitter()
{
  return m_emitter;
}

const IParticleEmitter *vkParticleState::GetEmitter() const
{
  return m_emitter;
}

void vkParticleState::SetStepper(IParticleStepper *stepper)
{
  VK_SET(m_stepper,  stepper);
}

IParticleStepper *vkParticleState::GetStepper()
{
  return m_stepper;
}

const IParticleStepper *vkParticleState::GetStepper() const
{
  return m_stepper;
}


void vkParticleState::Render(IGraphics *graphics, vkRenderPass pass)  const
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

void vkParticleState::PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config)
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



