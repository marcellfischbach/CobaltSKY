#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Entity/RenderState.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Entity/ParticleState.refl.hh>
#include <Valkyrie/Graphics/Particle.hh>
#include <Valkyrie/Math/RandomRange.hh>

class vkParticle;
class vkMaterialInstance;
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

VK_INTERFACE()
struct VKE_API IParticleStepper : public IObject
{
  VK_CLASS_GEN;

  virtual ~IParticleStepper() { }

  virtual void Update(float tpf, vkParticle *particle) = 0;
};


VK_CLASS()
class VKE_API vkParticleState : public vkRenderState
{
  VK_CLASS_GEN;
public:
  vkParticleState();
  virtual ~vkParticleState();

  void SetParticle(vkParticle *particle);
  vkParticle *GetParticle();
  const vkParticle *GetParticle() const;

  void SetMaterial(vkMaterialInstance *materialInstance);
  vkMaterialInstance *GetMaterial();
  const vkMaterialInstance *GetMaterial() const;

  void SetShadingMode(vkParticleShadingMode shadingMode);
  vkParticleShadingMode GetShadingMode() const;

  void SetEmitter(IParticleEmitter *emitter);
  IParticleEmitter *GetEmitter();
  const IParticleEmitter *GetEmitter() const;

  void SetStepper(IParticleStepper *emitter);
  IParticleStepper *GetStepper();
  const IParticleStepper *GetStepper() const;


  virtual void Update(float tpf);
  virtual void Render(IGraphics *graphics, vkRenderPass pass) const;
  virtual void PrivScan(vkClipper *clipper, IGraphics *graphics, IEntityScan *entityScan, const vkScanConfig &config);


private:
  bool m_castShadow;
  vkParticle *m_particle;
  vkMaterialInstance *m_materialInstance;

  vkParticleShadingMode m_shadingMode;

  IParticleEmitter *m_emitter;
  IParticleStepper *m_stepper;

};

enum vkParticleSpawnMode
{
  ePSM_Point,
  ePSM_Box,
  ePSM_Sphere,
};

enum vkParticleSizeMode
{
  ePSM_Constant,
  ePSM_Linear,
  ePSM_Triangle,
  ePSM_Saw,
  ePSM_Pulse,
};

enum vkParticleRotationMode
{
  ePRM_Pos,
  ePRM_Neg,
  ePRM_Both,
};


VK_CLASS()
class VKE_API vkDefaultParticleEmitter : public IParticleEmitter
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


VK_CLASS()
class VKE_API vkDefaultParticleStepper : public IParticleStepper
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

