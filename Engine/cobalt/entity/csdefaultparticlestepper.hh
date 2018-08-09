#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/iparticlestepper.hh>
#include <cobalt/csenums.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/graphics/csparticle.hh>

#include <cobalt/entity/csdefaultparticlestepper.refl.hh>

CS_CLASS()
class CSE_API csDefaultParticleStepper : public CS_SUPER(iParticleStepper)
{
  CS_CLASS_GEN_OBJECT;
public:
  csDefaultParticleStepper();
  virtual ~csDefaultParticleStepper();

  void SetSizeMode(csParticleSizeMode sizeMode);
  csParticleSizeMode GetSizeMode() const;

  void SetSizeCicleTime(float sizeCicleTime);
  float GetSizeCicleTime() const;

  void SetNumberOfTextPages(csUInt8 numTextPages);
  csUInt8 GetNumberOfTextPages() const;

  void SetTextPageTime(float textPageTime);
  float GetTextPageTime() const;

  void SetGravity(const csVector3f &gravity);
  const csVector3f &GetGravity() const;

  void Update(float tpf, csParticle *particle);

protected:
  virtual void UpdateParticle(float tpf, csParticle::ParticleData *particle);


private:
  csParticleSizeMode m_sizeMode;
  float m_sizeCicleTime;
  csUInt8 m_numTextPages;
  float m_textPageTime;
  csVector3f m_gravity;
};

