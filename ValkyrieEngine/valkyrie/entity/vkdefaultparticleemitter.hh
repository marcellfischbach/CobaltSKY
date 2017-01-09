#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/entity/iparticleemitter.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/math/vkrandomrange.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/vkenums.hh>

#include <valkyrie/entity/vkdefaultparticleemitter.refl.hh>

VK_CLASS()
class VKE_API vkDefaultParticleEmitter : public VK_SUPER(iParticleEmitter)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkDefaultParticleEmitter();
  virtual ~vkDefaultParticleEmitter();

  void SetParticlesPerSecond(float particlesPerSecond);
  float GetParticlesPerSecond() const;

  void SetInitialTime(const vkRandomRange &initialTime);
  const vkRandomRange &GetInitialTime() const;

  void SetTimeToLive(const vkRandomRange &timeToLive);
  const vkRandomRange &GetTimeToLive() const;

  void SetSpawnPos(const vkVector3f &pos);
  void SetSpawnBox(const vkVector3f &min, const vkVector3f &max);
  void SetSpawnSphere(const vkVector3f &center, const vkRandomRange &radius);

  void SetInitialDirection(const vkVector3f &direction, const vkRandomRange &angle, const vkRandomRange &speed);
  const vkVector3f &GetInitialDirection() const;
  const vkRandomRange &GetInitialDirectionAngle() const;
  const vkRandomRange &GetInitialDirectionSpeed() const;

  void SetInitialRotation(const vkRandomRange &randomRange);
  const vkRandomRange &GetInitialRotation() const;

  void SetRotationSpeed(const vkRandomRange &rotationSpeed);
  const vkRandomRange &GetRotationSpeed() const;

  void SetRotationMode(vkParticleRotationMode rotationMode);
  vkParticleRotationMode GetRotationMode() const;

  void SetSizeX(const vkRandomRange &sizeX);
  const vkRandomRange &GetSizeX() const;

  void SetSizeY(const vkRandomRange &sizeZ);
  const vkRandomRange &GetSizeY() const;

  void SetSyncSize(bool syncSize);
  bool IsSyncSize() const;

  void Update(float tpf, vkParticle *particle);

private:

  float m_particlesPerSecond;
  float m_timeToNextParticle;

  vkRandomRange m_initialTime;
  vkRandomRange m_timeToLive;

  vkParticleSpawnMode m_spawnMode;
  vkVector3f m_spawnPos;
  vkVector3f m_spawnBoxMin;
  vkVector3f m_spawnBoxMax;
  vkRandomRange m_spawnRadius;

  vkVector3f m_initialDirection;
  vkRandomRange m_initialDirectionAngle;
  vkRandomRange m_initialDirectionSpeed;
  vkMatrix3f m_initialDirectionMatrix;

  vkRandomRange m_initialRotation;
  vkRandomRange m_rotationSpeed;
  vkParticleRotationMode m_rotationMode;
  vkRandomRange m_sizeX;
  vkRandomRange m_sizeY;
  bool m_syncSize;
};
