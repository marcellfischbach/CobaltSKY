#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/entity/iparticlestepper.refl.hh>


namespace cs
{

class Particle;
CS_CLASS()
struct CSE_API iParticleStepper : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleStepper() { }

  virtual void Update(float tpf, cs::Particle* particle) = 0;
};

}
