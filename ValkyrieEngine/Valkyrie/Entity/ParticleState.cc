
#include <Valkyrie/Entity/ParticleState.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/Particle.hh>
#include <Valkyrie/Entity/Scan.hh>

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



vkDefaultParticleEmitter::vkDefaultParticleEmitter()
  : IParticleEmitter()
  , m_particlesPerSecond(0)
  , m_timeToLive(vkRandomRange::Value(10.0f))
  , m_initialRotation(vkRandomRange::Value(0.0f))
  , m_rotationSpeed(vkRandomRange::Value(0.0f))
{
  VK_CLASS_GEN_CONSTR;
  SetParticlesPerSecond(20.0f);
}

vkDefaultParticleEmitter::~vkDefaultParticleEmitter()
{

}

void vkDefaultParticleEmitter::SetParticlesPerSecond(float particlesPerSecond)
{
  m_particlesPerSecond = particlesPerSecond;
  m_timeToNextParticle = 1.0f / m_particlesPerSecond;
}

float vkDefaultParticleEmitter::GetParticlesPerSecond() const
{
  return m_particlesPerSecond;
}

void vkDefaultParticleEmitter::SetTimeToLive(const vkRandomRange& timeToLive)
{
  m_timeToLive = timeToLive;
}

const vkRandomRange &vkDefaultParticleEmitter::GetTimeToLive() const
{
  return m_timeToLive;
}

void vkDefaultParticleEmitter::SetInitialRotation(const vkRandomRange &initialRotation)
{
  m_initialRotation = initialRotation;
}

const vkRandomRange &vkDefaultParticleEmitter::GetInitialRotation() const
{
  return m_initialRotation;
}

void vkDefaultParticleEmitter::SetRotationSpeed(const vkRandomRange &rotationSpeed)
{
  m_rotationSpeed = rotationSpeed;
}

const vkRandomRange&vkDefaultParticleEmitter::GetRotationSpeed() const
{
  return m_rotationSpeed;
}

void vkDefaultParticleEmitter::SetSizeX(const vkRandomRange &sizeX)
{
  m_sizeX = sizeX;
}

const vkRandomRange &vkDefaultParticleEmitter::GetSizeX() const
{
  return m_sizeX;
}

void vkDefaultParticleEmitter::SetSizeY(const vkRandomRange &sizeY)
{
  m_sizeY = sizeY;
}

const vkRandomRange &vkDefaultParticleEmitter::GetSizeY() const
{
  return m_sizeY;
}

void vkDefaultParticleEmitter::Update(float tpf, vkParticle *particle)
{
  vkParticle::ParticleData *data = 0;
  m_timeToNextParticle -= tpf;
  vkSize particleID = 0;
  while (true)
  {
    if (m_timeToNextParticle > 0.0f)
    {
      break;
    }

    m_timeToNextParticle += 1.0f / m_particlesPerSecond;
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


        x = (float)rand() / (float)RAND_MAX;
        y = (float)rand() / (float)RAND_MAX;
        z = (float)rand() / (float)RAND_MAX;
        data[particleID].direction = vkVector3f(-10.0f + x * 20.0f, -10.0f + y * 20.0f, 10.0f + z * 15.0f);


        vkRandomRange sizeX = vkRandomRange::Range(m_sizeX.Get(), m_sizeX.Get());
        vkRandomRange sizeY = vkRandomRange::Range(m_sizeY.Get(), m_sizeY.Get());

        data[particleID].size = vkVector2f(sizeX.Get(), sizeX.Get());
        data[particleID].sizeRange = vkVector4f(sizeX.GetMin(), sizeX.GetRange(), sizeY.GetMin(), sizeY.GetRange());
        data[particleID].rotation = m_initialRotation.Get();
        data[particleID].rotationSpeed = m_rotationSpeed.Get();
        data[particleID].timeToLive = m_timeToLive.Get();
        break;
      }
    }

  }
  if (data)
  {
    particle->UnlockParticleData();
  }

}

vkDefaultParticleStepper::vkDefaultParticleStepper()
  : IParticleStepper()
  , m_sizeMode(ePSM_Linear)
{
  VK_CLASS_GEN_CONSTR;
}

vkDefaultParticleStepper::~vkDefaultParticleStepper()
{

}

void vkDefaultParticleStepper::SetSizeMode(vkParticleSizeMode sizeMode)
{
  m_sizeMode = sizeMode;
}

vkParticleSizeMode vkDefaultParticleStepper::GetSizeMode() const
{
  return m_sizeMode;
}

void vkDefaultParticleStepper::Update(float tpf, vkParticle *particle)
{
  vkParticle::ParticleData *data;
  if (particle->GetParticleBuffer()->Lock(0, (void **)&data, eBAM_ReadWrite))
  {
    for (unsigned i = 0; i < particle->GetNumberOfParticles(); ++i)
    {
      vkParticle::ParticleData *part = &data[i];
      if (part->timeToLive > 0.0f)
      {
        UpdateParticle(tpf, part);
        part->timeToLive -= tpf;
      }
    }
    particle->GetParticleBuffer()->Unlock();
  }

}

void vkDefaultParticleStepper::UpdateParticle(float tpf, vkParticle::ParticleData* particle)
{
  //particle->direction += gr;
  particle->rotation += particle->rotationSpeed * tpf;
  vkVector3f::MulAdd(particle->position, particle->direction, tpf, particle->position);


  if (particle->position.z < -2.0f)
  {
    particle->timeToLive = -1.0f;
  }

}

vkGravitationParticleStepper::vkGravitationParticleStepper()
  : vkDefaultParticleStepper()
  , m_gravity(0.0f, 0.0f, -9.81f)
{
}

vkGravitationParticleStepper::~vkGravitationParticleStepper()
{

}


void vkGravitationParticleStepper::SetGravity(const vkVector3f &gravity)
{
  m_gravity = gravity;
}

const vkVector3f &vkGravitationParticleStepper::GetGravity() const
{
  return m_gravity;
}


void vkGravitationParticleStepper::UpdateParticle(float tpf, vkParticle::ParticleData *particle)
{
    vkVector3f gr(m_gravity);
    gr *= tpf;

    particle->direction += gr;
    vkDefaultParticleStepper::UpdateParticle(tpf, particle);
}