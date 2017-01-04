#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>

#include <Valkyrie/entity/iparticlestepper.refl.hh>

class vkParticle;

VK_INTERFACE()
struct VKE_API IParticleStepper : public IObject
{
  VK_CLASS_GEN;

  virtual ~IParticleStepper() { }

  virtual void Update(float tpf, vkParticle *particle) = 0;
};

