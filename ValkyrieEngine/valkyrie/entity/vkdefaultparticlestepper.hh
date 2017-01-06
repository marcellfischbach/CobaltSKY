#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/iparticlestepper.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/graphics/vkparticle.hh>

#include <valkyrie/entity/vkdefaultparticlestepper.refl.hh>

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

