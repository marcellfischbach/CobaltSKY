#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>

#include <valkyrie/entity/iparticleemitter.refl.hh>

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
