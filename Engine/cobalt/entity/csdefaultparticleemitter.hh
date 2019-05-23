#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/entity/iparticleemitter.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csrandomrange.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>

#include <cobalt/entity/csdefaultparticleemitter.refl.hh>

namespace cs
{

CS_CLASS()
class CSE_API DefaultParticleEmitter : public CS_SUPER(cs::iParticleEmitter)
{
  CS_CLASS_GEN_OBJECT;
public:
  DefaultParticleEmitter();
  virtual ~DefaultParticleEmitter();

  void SetParticlesPerSecond(float particlesPerSecond);
  float GetParticlesPerSecond() const;

  void SetInitialTime(const cs::RandomRange & initialTime);
  const cs::RandomRange& GetInitialTime() const;

  void SetTimeToLive(const cs::RandomRange & timeToLive);
  const cs::RandomRange& GetTimeToLive() const;

  void SetSpawnPos(const cs::Vector3f & pos);
  void SetSpawnBox(const cs::Vector3f & min, const cs::Vector3f & max);
  void SetSpawnSphere(const cs::Vector3f & center, const cs::RandomRange & radius);

  void SetInitialDirection(const cs::Vector3f & direction, const cs::RandomRange & angle, const cs::RandomRange & speed);
  const cs::Vector3f& GetInitialDirection() const;
  const cs::RandomRange& GetInitialDirectionAngle() const;
  const cs::RandomRange& GetInitialDirectionSpeed() const;

  void SetInitialRotation(const cs::RandomRange & randomRange);
  const cs::RandomRange& GetInitialRotation() const;

  void SetRotationSpeed(const cs::RandomRange & rotationSpeed);
  const cs::RandomRange& GetRotationSpeed() const;

  void SetRotationMode(cs::eParticleRotationMode rotationMode);
  cs::eParticleRotationMode GetRotationMode() const;

  void SetSizeX(const cs::RandomRange & sizeX);
  const cs::RandomRange& GetSizeX() const;

  void SetSizeY(const cs::RandomRange & sizeZ);
  const cs::RandomRange& GetSizeY() const;

  void SetSyncSize(bool syncSize);
  bool IsSyncSize() const;

  void Update(float tpf, cs::Particle * particle);

private:

  float m_particlesPerSecond;
  float m_timeToNextParticle;

  cs::RandomRange m_initialTime;
  cs::RandomRange m_timeToLive;

  cs::eParticleSpawnMode m_spawnMode;
  cs::Vector3f m_spawnPos;
  cs::Vector3f m_spawnBoxMin;
  cs::Vector3f m_spawnBoxMax;
  cs::RandomRange m_spawnRadius;

  cs::Vector3f m_initialDirection;
  cs::RandomRange m_initialDirectionAngle;
  cs::RandomRange m_initialDirectionSpeed;
  cs::Matrix3f m_initialDirectionMatrix;

  cs::RandomRange m_initialRotation;
  cs::RandomRange m_rotationSpeed;
  cs::eParticleRotationMode m_rotationMode;
  cs::RandomRange m_sizeX;
  cs::RandomRange m_sizeY;
  bool m_syncSize;
};

}

