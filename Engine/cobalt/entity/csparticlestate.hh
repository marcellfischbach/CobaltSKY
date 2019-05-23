#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/entity/iparticleemitter.hh>
#include <cobalt/entity/iparticlestepper.hh>
#include <cobalt/entity/csrenderstate.hh>
#include <cobalt/csenums.hh>

#include <cobalt/entity/csparticlestate.refl.hh>

class cs::Particle;


namespace cs
{
class Material;

CS_CLASS()
class CSE_API ParticleState : public CS_SUPER(cs::RenderState)
{
  CS_CLASS_GEN;
public:
  ParticleState();
  virtual ~ParticleState();

  void SetParticle(cs::Particle * particle);
  cs::Particle* GetParticle();
  const cs::Particle* GetParticle() const;

  void SetMaterial(cs::Material * materialInstance);
  cs::Material* GetMaterial();
  const cs::Material* GetMaterial() const;

  void SetShadingMode(cs::eParticleShadingMode shadingMode);
  cs::eParticleShadingMode GetShadingMode() const;

  void SetEmitter(cs::iParticleEmitter * emitter);
  cs::iParticleEmitter* GetEmitter();
  const cs::iParticleEmitter* GetEmitter() const;

  void SetStepper(cs::iParticleStepper * emitter);
  cs::iParticleStepper* GetStepper();
  const cs::iParticleStepper* GetStepper() const;


  virtual void Update(float tpf);
  virtual void Render(cs::iGraphics * graphics, cs::eRenderPass pass) const;
  virtual void PrivScan(cs::Clipper * clipper, cs::iGraphics * graphics, cs::iEntityScan * entityScan, const cs::ScanConfig & config);


private:
  bool m_castShadow;
  cs::Particle* m_particle;
  cs::Material* m_materialInstance;

  cs::eParticleShadingMode m_shadingMode;

  cs::iParticleEmitter* m_emitter;
  cs::iParticleStepper* m_stepper;

};

}