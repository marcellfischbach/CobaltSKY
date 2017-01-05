#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/Math/Vector.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Graphics/Particle.refl.hh>

struct IGraphics;
struct IVertexBuffer;
struct IVertexDeclaration;

class vkMaterialInstance;
class vkParticle;


VK_CLASS()
class VKE_API vkParticle : public VK_SUPER(vkObject)
{
  VK_CLASS_GEN;

public:
  struct ParticleData
  {
    vkVector3f position;
    vkVector3f direction;
    vkVector2f size;
    vkVector4f sizeRange;
    float      sizeTime;
    float      rotation;
    float      rotationSpeed;
    float      time;
    float      timeToLive;
    vkVector3f textPage;
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




