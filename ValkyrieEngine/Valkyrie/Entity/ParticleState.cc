
#include <Valkyrie/Entity/ParticleState.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Particle.hh>
#include <Valkyrie/Entity/Scan.hh>

vkParticleState::vkParticleState()
  : vkRenderState()
  , m_particle(0)
  , m_materialInstance(0)
  , m_shadingMode(ePSM_Emitting)
{

}

vkParticleState::~vkParticleState()
{
  VK_RELEASE(m_particle);
  VK_RELEASE(m_materialInstance);
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











vkParticleEmitterState::vkParticleEmitterState()
  : vkSpatialState()
  , m_particleState(0)
  , m_particlesPerSecond(20.0f)
{
  m_timeToNextParticle = 1.0f / m_particlesPerSecond;
}

vkParticleEmitterState::~vkParticleEmitterState()
{
  VK_RELEASE(m_particleState);
}

void vkParticleEmitterState::SetParticleState(vkParticleState *particleState)
{
  VK_SET(m_particleState, particleState);
}

vkParticleState *vkParticleEmitterState::GetParticleState()
{
  return m_particleState;
}

const vkParticleState *vkParticleEmitterState::GetParticleState() const
{
  return m_particleState;
}


void vkParticleEmitterState::Update(float tpf)
{
  if (!m_particleState)
  {
    return;
  }

  vkParticle *particle = m_particleState->GetParticle();
  if (!particle)
  {
    return;
  }

  vkParticle::ParticleData *data = 0;
  m_timeToNextParticle -= tpf;
  vkSize particleID = 0;
  while (true)
  {
    if (m_timeToNextParticle > 0.0f)
    {
      break;
    }

    m_timeToNextParticle += 1.0 / m_particlesPerSecond;
    if (!data)
    {
      particle->LockParticleData(false);
      data = particle->GetParticleData();
    }

    for (vkSize in = particle->GetNumberOfParticles(); particleID < in; ++particleID)
    {
      if (data[particleID].timeToLive < 0.0)
      {
        float x = (float)rand() / (float)RAND_MAX;
        float y = (float)rand() / (float)RAND_MAX;
        float z = (float)rand() / (float)RAND_MAX;
        data[particleID].position = vkVector3f(-1.0f + x * 2.0f, -1.0f + y * 2.0f, z * 0.0f);
        data[particleID].size = vkVector2f(2.0f, 2.0f);
        data[particleID].rotation = 0.0f;
        data[particleID].timeToLive = 10.0f;
        break;
      }
    }

  }
  if (data)
  {
    particle->UnlockParticleData();
  }

}