#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/iparticlestepper.refl.hh>

class csParticle;

CS_INTERFACE()
struct CSE_API iParticleStepper : public iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleStepper() { }

  virtual void Update(float tpf, csParticle *particle) = 0;
};

