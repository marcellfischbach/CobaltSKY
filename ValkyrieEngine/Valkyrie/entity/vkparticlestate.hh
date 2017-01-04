#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/entity/iparticleemitter.hh>
#include <Valkyrie/entity/iparticlestepper.hh>
#include <Valkyrie/entity/vkrenderstate.hh>
#include <Valkyrie/vkenums.hh>

#include <Valkyrie/entity/vkparticlestate.refl.hh>

class vkParticle;
class vkMaterialInstance;




VK_CLASS()
class VKE_API vkParticleState : public VK_SUPER(vkRenderState)
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

  void SetEmitter(IParticleEmitter *emitter);
  IParticleEmitter *GetEmitter();
  const IParticleEmitter *GetEmitter() const;

  void SetStepper(IParticleStepper *emitter);
  IParticleStepper *GetStepper();
  const IParticleStepper *GetStepper() const;


  virtual void Update(float tpf);
  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);


private:
  bool m_castShadow;
  vkParticle *m_particle;
  vkMaterialInstance *m_materialInstance;

  vkParticleShadingMode m_shadingMode;

  IParticleEmitter *m_emitter;
  IParticleStepper *m_stepper;

};
