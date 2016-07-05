#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Entity/ParticleState.refl.hh>

class vkParticle;
class vkMaterialInstance;
/**
* \ingroup entity
*/

VK_CLASS()
class VKE_API vkParticleState : public vkRenderState
{
  VK_CLASS_GEN;
public:
  vkParticleState();
  virtual ~vkParticleState();

  void SetParticle(vkParticle *particle);
  vkParticle *GetParticle();
  const vkParticle *GetParticle() const;

  void SetMaterial(vkMaterialInstance *materialInstance);
  vkMaterialInstance *GetMaterial();
  const vkMaterialInstance *GetMaterial() const;

  void SetShadingMode(vkParticleShadingMode shadingMode);
  vkParticleShadingMode GetShadingMode() const;

  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);

private:
  bool m_castShadow;
  vkParticle *m_particle;
  vkMaterialInstance *m_materialInstance;

  vkParticleShadingMode m_shadingMode;
};


VK_CLASS()
class VKE_API vkParticleEmitterState : public vkSpatialState
{
  VK_CLASS_GEN;
public:
  vkParticleEmitterState();
  virtual ~vkParticleEmitterState();

  void SetParticleState(vkParticleState *particleState);
  vkParticleState *GetParticleState();
  const vkParticleState *GetParticleState() const;

  virtual void Update(float tpf);

private:
  vkParticleState *m_particleState;


  float m_particlesPerSecond;
  float m_timeToNextParticle;

};