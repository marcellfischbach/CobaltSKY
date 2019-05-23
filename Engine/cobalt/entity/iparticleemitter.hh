#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>

#include <cobalt/entity/iparticleemitter.refl.hh>

/**
* \ingroup entity
*/

namespace cs
{
class Particle;

CS_CLASS()
struct CSE_API iParticleEmitter : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleEmitter() { }

  virtual void Update(float tpf, cs::Particle* particle) = 0;

};

}
