#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/entity/iparticleemitter.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csrandomrange.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>

#include <cobalt/entity/csdefaultparticleemitter.refl.hh>

CS_CLASS()
class CSE_API csDefaultParticleEmitter : public CS_SUPER(iParticleEmitter)
{
  CS_CLASS_GEN_OBJECT;
public:
  csDefaultParticleEmitter();
  virtual ~csDefaultParticleEmitter();

  void SetParticlesPerSecond(float particlesPerSecond);
  float GetParticlesPerSecond() const;

  void SetInitialTime(const csRandomRange &initialTime);
  const csRandomRange &GetInitialTime() const;

  void SetTimeToLive(const csRandomRange &timeToLive);
  const csRandomRange &GetTimeToLive() const;

  void SetSpawnPos(const csVector3f &pos);
  void SetSpawnBox(const csVector3f &min, const csVector3f &max);
  void SetSpawnSphere(const csVector3f &center, const csRandomRange &radius);

  void SetInitialDirection(const csVector3f &direction, const csRandomRange &angle, const csRandomRange &speed);
  const csVector3f &GetInitialDirection() const;
  const csRandomRange &GetInitialDirectionAngle() const;
  const csRandomRange &GetInitialDirectionSpeed() const;

  void SetInitialRotation(const csRandomRange &randomRange);
  const csRandomRange &GetInitialRotation() const;

  void SetRotationSpeed(const csRandomRange &rotationSpeed);
  const csRandomRange &GetRotationSpeed() const;

  void SetRotationMode(csParticleRotationMode rotationMode);
  csParticleRotationMode GetRotationMode() const;

  void SetSizeX(const csRandomRange &sizeX);
  const csRandomRange &GetSizeX() const;

  void SetSizeY(const csRandomRange &sizeZ);
  const csRandomRange &GetSizeY() const;

  void SetSyncSize(bool syncSize);
  bool IsSyncSize() const;

  void Update(float tpf, csParticle *particle);

private:

  float m_particlesPerSecond;
  float m_timeToNextParticle;

  csRandomRange m_initialTime;
  csRandomRange m_timeToLive;

  csParticleSpawnMode m_spawnMode;
  csVector3f m_spawnPos;
  csVector3f m_spawnBoxMin;
  csVector3f m_spawnBoxMax;
  csRandomRange m_spawnRadius;

  csVector3f m_initialDirection;
  csRandomRange m_initialDirectionAngle;
  csRandomRange m_initialDirectionSpeed;
  csMatrix3f m_initialDirectionMatrix;

  csRandomRange m_initialRotation;
  csRandomRange m_rotationSpeed;
  csParticleRotationMode m_rotationMode;
  csRandomRange m_sizeX;
  csRandomRange m_sizeY;
  bool m_syncSize;
};
