#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/vkparticle.refl.hh>

struct iGraphics;
struct iVertexBuffer;
struct iVertexDeclaration;

class vkMaterial;
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

  bool Initialize(iGraphics *renderer, vkSize numberOfParticles);

  void SetParticleData(unsigned numParticles, const ParticleData *data);
  iVertexBuffer *GetParticleBuffer();
  const iVertexBuffer *GetParticleBuffer() const;

  void SetNumberOfParticles(vkSize numParticles);
  vkSize GetNumberOfParticles() const;

  void SetNumberOfRenderParticles(vkSize numParticles);
  vkSize GetNumberOfRenderParticles() const;

  bool LockParticleData(bool readOnly = false);
  ParticleData *GetParticleData();
  const ParticleData *GetParticleData() const;
  void UnlockParticleData();

  void Render(iGraphics *renderer, vkRenderPass pass, vkMaterial *material);


private:
  iVertexDeclaration *m_vertexDeclaration;
  iVertexBuffer *m_buffer;

  vkSize m_numParticles;
  vkSize m_numRenderParticles;
  ParticleData *m_particleData;
};




