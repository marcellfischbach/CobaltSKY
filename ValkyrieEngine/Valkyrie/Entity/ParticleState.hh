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


enum vkParticleSizeMode
{
  ePSM_Linear,
  ePSM_Triangle,
  ePSM_Saw,
  ePSM_Pulse,
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

  void SetTimeToLive(const vkRandomRange &timeToLive);
  const vkRandomRange &GetTimeToLive() const;

  void SetInitialRotation(const vkRandomRange &randomRange);
  const vkRandomRange &GetInitialRotation() const;

  void SetRotationSpeed(const vkRandomRange &rotationSpeed);
  const vkRandomRange &GetRotationSpeed() const;

  void SetSizeX(const vkRandomRange &sizeX);
  const vkRandomRange &GetSizeX() const;

  void SetSizeY(const vkRandomRange &sizeZ);
  const vkRandomRange &GetSizeY() const;

  void Update(float tpf, vkParticle *particle);

private:

  float m_particlesPerSecond;
  float m_timeToNextParticle;

  vkRandomRange m_timeToLive;
  vkRandomRange m_initialRotation;
  vkRandomRange m_rotationSpeed;
  vkRandomRange m_sizeX;
  vkRandomRange m_sizeY;
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

  void Update(float tpf, vkParticle *particle);

protected:
  virtual void UpdateParticle(float tpf, vkParticle::ParticleData *particle);


private:
  vkParticleSizeMode m_sizeMode;

};

VK_CLASS()
class VKE_API vkGravitationParticleStepper : public vkDefaultParticleStepper
{
  VK_CLASS_GEN;
public:
  vkGravitationParticleStepper();
  virtual ~vkGravitationParticleStepper();

  void SetGravity(const vkVector3f &gravity);
  const vkVector3f &GetGravity() const;

protected:
  virtual void UpdateParticle(float tpf, vkParticle::ParticleData *particle);


private:
  vkVector3f m_gravity;

};