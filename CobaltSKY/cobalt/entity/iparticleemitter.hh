#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>

#include <cobalt/entity/iparticleemitter.refl.hh>

class csParticle;
/**
* \ingroup entity
*/

CS_INTERFACE()
struct CSE_API iParticleEmitter : public iObject
{
  CS_CLASS_GEN;

  virtual ~iParticleEmitter() { }

  virtual void Update(float tpf, csParticle *particle) = 0;

};
