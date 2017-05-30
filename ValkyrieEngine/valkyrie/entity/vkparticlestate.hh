#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/entity/iparticleemitter.hh>
#include <valkyrie/entity/iparticlestepper.hh>
#include <valkyrie/entity/vkrenderstate.hh>
#include <valkyrie/vkenums.hh>

#include <valkyrie/entity/vkparticlestate.refl.hh>

class vkParticle;
class vkMaterial;




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

  void SetMaterial(vkMaterial *materialInstance);
  vkMaterial *GetMaterial();
  const vkMaterial *GetMaterial() const;

  void SetShadingMode(vkParticleShadingMode shadingMode);
  vkParticleShadingMode GetShadingMode() const;

  void SetEmitter(iParticleEmitter *emitter);
  iParticleEmitter *GetEmitter();
  const iParticleEmitter *GetEmitter() const;

  void SetStepper(iParticleStepper *emitter);
  iParticleStepper *GetStepper();
  const iParticleStepper *GetStepper() const;


  virtual void Update(float tpf);
  virtual void Render(iGraphics *graphics, vkRenderPass pass) const;
  virtual void PrivScan(vkClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const vkScanConfig &config);


private:
  bool m_castShadow;
  vkParticle *m_particle;
  vkMaterial *m_materialInstance;

  vkParticleShadingMode m_shadingMode;

  iParticleEmitter *m_emitter;
  iParticleStepper *m_stepper;

};
