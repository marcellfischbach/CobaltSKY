#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>

#include <valkyrie/entity/iparticleemitter.refl.hh>

class vkParticle;
/**
* \ingroup entity
*/

VK_INTERFACE()
struct VKE_API IParticleEmitter : public IObject
{
  VK_CLASS_GEN;

  virtual ~IParticleEmitter() { }

  virtual void Update(float tpf, vkParticle *particle) = 0;

};
