#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>

#include <valkyrie/entity/iparticleemitter.refl.hh>

class vkParticle;
/**
* \ingroup entity
*/

VK_INTERFACE()
struct VKE_API iParticleEmitter : public iObject
{
  VK_CLASS_GEN;

  virtual ~iParticleEmitter() { }

  virtual void Update(float tpf, vkParticle *particle) = 0;

};
