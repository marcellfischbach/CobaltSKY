#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/entity/iparticlestepper.refl.hh>

class csParticle;

CS_CLASS()
struct CSE_API iParticleStepper : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleStepper() { }

  virtual void Update(float tpf, csParticle *particle) = 0;
};

