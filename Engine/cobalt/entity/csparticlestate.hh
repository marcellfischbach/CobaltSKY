#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/iparticleemitter.hh>
#include <cobalt/entity/iparticlestepper.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/csenums.hh>

#include <cobalt/entity/csparticlestate.refl.hh>

class csParticle;
class csMaterial;




CS_CLASS()
class CSE_API csParticleState : public CS_SUPER(csRenderState)
{
  CS_CLASS_GEN;
public:
  csParticleState();
  virtual ~csParticleState();

  void SetParticle(csParticle *particle);
  csParticle *GetParticle();
  const csParticle *GetParticle() const;

  void SetMaterial(csMaterial *materialInstance);
  csMaterial *GetMaterial();
  const csMaterial *GetMaterial() const;

  void SetShadingMode(csParticleShadingMode shadingMode);
  csParticleShadingMode GetShadingMode() const;

  void SetEmitter(iParticleEmitter *emitter);
  iParticleEmitter *GetEmitter();
  const iParticleEmitter *GetEmitter() const;

  void SetStepper(iParticleStepper *emitter);
  iParticleStepper *GetStepper();
  const iParticleStepper *GetStepper() const;


  virtual void Update(float tpf);
  virtual void Render(iGraphics *graphics, csRenderPass pass) const;
  virtual void PrivScan(csClipper *clipper, iGraphics *graphics, iEntityScan *entityScan, const csScanConfig &config);


private:
  bool m_castShadow;
  csParticle *m_particle;
  csMaterial *m_materialInstance;

  csParticleShadingMode m_shadingMode;

  iParticleEmitter *m_emitter;
  iParticleStepper *m_stepper;

};
