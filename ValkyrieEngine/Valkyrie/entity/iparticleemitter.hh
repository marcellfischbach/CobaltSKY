#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>

#include <Valkyrie/entity/iparticleemitter.refl.hh>

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
