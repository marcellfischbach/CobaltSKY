#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>

#include <valkyrie/entity/iparticlestepper.refl.hh>

class csParticle;

CS_INTERFACE()
struct CSE_API iParticleStepper : public iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleStepper() { }

  virtual void Update(float tpf, csParticle *particle) = 0;
};

