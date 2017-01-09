#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>

#include <valkyrie/entity/iparticlestepper.refl.hh>

class vkParticle;

VK_INTERFACE()
struct VKE_API iParticleStepper : public iObject
{
  VK_CLASS_GEN;

  virtual ~iParticleStepper() { }

  virtual void Update(float tpf, vkParticle *particle) = 0;
};

