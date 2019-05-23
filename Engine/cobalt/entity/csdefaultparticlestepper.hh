#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/iparticlestepper.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/csparticle.hh>

#include <cobalt/entity/csdefaultparticlestepper.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API DefaultParticleStepper : public CS_SUPER(cs::iParticleStepper)
{
  CS_CLASS_GEN_OBJECT;
public:
  DefaultParticleStepper();
  virtual ~DefaultParticleStepper();

  void SetSizeMode(cs::eParticleSizeMode sizeMode);
  cs::eParticleSizeMode GetSizeMode() const;

  void SetSizeCicleTime(float sizeCicleTime);
  float GetSizeCicleTime() const;

  void SetNumberOfTextPages(csUInt8 numTextPages);
  csUInt8 GetNumberOfTextPages() const;

  void SetTextPageTime(float textPageTime);
  float GetTextPageTime() const;

  void SetGravity(const cs::Vector3f & gravity);
  const cs::Vector3f& GetGravity() const;

  void Update(float tpf, cs::Particle * particle);

protected:
  virtual void UpdateParticle(float tpf, cs::Particle::ParticleData * particle);


private:
  cs::eParticleSizeMode m_sizeMode;
  float m_sizeCicleTime;
  csUInt8 m_numTextPages;
  float m_textPageTime;
  cs::Vector3f m_gravity;
};


}

