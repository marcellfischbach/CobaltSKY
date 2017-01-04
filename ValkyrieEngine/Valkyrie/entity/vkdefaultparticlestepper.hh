#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/entity/iparticlestepper.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Graphics/Particle.hh>

#include <Valkyrie/entity/vkdefaultparticlestepper.refl.hh>

VK_CLASS()
class VKE_API vkDefaultParticleStepper : public VK_SUPER(IParticleStepper)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDefaultParticleStepper();
  virtual ~vkDefaultParticleStepper();

  void SetSizeMode(vkParticleSizeMode sizeMode);
  vkParticleSizeMode GetSizeMode() const;

  void SetSizeCicleTime(float sizeCicleTime);
  float GetSizeCicleTime() const;

  void SetNumberOfTextPages(vkUInt8 numTextPages);
  vkUInt8 GetNumberOfTextPages() const;

  void SetTextPageTime(float textPageTime);
  float GetTextPageTime() const;

  void SetGravity(const vkVector3f &gravity);
  const vkVector3f &GetGravity() const;

  void Update(float tpf, vkParticle *particle);

protected:
  virtual void UpdateParticle(float tpf, vkParticle::ParticleData *particle);


private:
  vkParticleSizeMode m_sizeMode;
  float m_sizeCicleTime;
  vkUInt8 m_numTextPages;
  float m_textPageTime;
  vkVector3f m_gravity;
};

