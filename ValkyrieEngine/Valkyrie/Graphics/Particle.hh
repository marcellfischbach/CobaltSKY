#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/Particle.refl.hh>

struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;

class vkMaterialInstance;
class vkParticle;


VK_CLASS()
class VKE_API vkParticle : public vkObject
{
  VK_CLASS_GEN;

public:
  struct ParticleData
  {
    vkVector3f position;
    vkVector3f direction;
    vkVector2f size;
    float      rotation;
    float      angularSpeed;
    float      timeToLive;
  };

public:
  vkParticle();
  virtual ~vkParticle();

  bool Initialize(IGraphics *renderer, vkSize numberOfParticles);

  void SetParticleData(unsigned numParticles, const ParticleData *data);
  IVertexBuffer *GetParticleBuffer();
  const IVertexBuffer *GetParticleBuffer() const;

  void SetNumberOfParticles(vkSize numParticles);
  vkSize GetNumberOfParticles() const;

  void SetNumberOfRenderParticles(vkSize numParticles);
  vkSize GetNumberOfRenderParticles() const;

  bool LockParticleData(bool readOnly = false);
  ParticleData *GetParticleData();
  const ParticleData *GetParticleData() const;
  void UnlockParticleData();

  void Render(IGraphics *renderer, vkRenderPass pass, vkMaterialInstance *material);


private:
  IVertexDeclaration *m_vertexDeclaration;
  IVertexBuffer *m_buffer;

  vkSize m_numParticles;
  vkSize m_numRenderParticles;
  ParticleData *m_particleData;
};




