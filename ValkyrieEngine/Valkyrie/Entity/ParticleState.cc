
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
