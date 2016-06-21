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

VK_CLASS()
class VKE_API vkParticle : public vkObject
{
  VK_CLASS_GEN;

public:
  struct ParticleData
  {
    vkVector3f position;
    vkVector2f size;
    float      rotation;
    vkUInt32   spawnTime;
    vkUInt32   killTime;
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


  void Render(IGraphics *renderer, vkRenderPass pass, vkMaterialInstance *material);


private:
  IVertexDeclaration *m_vertexDeclaration;
  IVertexBuffer *m_buffer;

  vkSize m_numParticles;
  vkSize m_numRenderParticles;
};