#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/csparticle.refl.hh>

struct iGraphics;
struct iVertexBuffer;
struct iVertexDeclaration;

class csMaterial;
class csParticle;


CS_CLASS()
class CSE_API csParticle : public CS_SUPER(csObject)
{
  CS_CLASS_GEN;

public:
  struct ParticleData
  {
    csVector3f position;
    csVector3f direction;
    csVector2f size;
    csVector4f sizeRange;
    float      sizeTime;
    float      rotation;
    float      rotationSpeed;
    float      time;
    float      timeToLive;
    csVector3f textPage;
  };

public:
  csParticle();
  virtual ~csParticle();

  bool Initialize(iGraphics *renderer, csSize numberOfParticles);

  void SetParticleData(unsigned numParticles, const ParticleData *data);
  iVertexBuffer *GetParticleBuffer();
  const iVertexBuffer *GetParticleBuffer() const;

  void SetNumberOfParticles(csSize numParticles);
  csSize GetNumberOfParticles() const;

  void SetNumberOfRenderParticles(csSize numParticles);
  csSize GetNumberOfRenderParticles() const;

  bool LockParticleData(bool readOnly = false);
  ParticleData *GetParticleData();
  const ParticleData *GetParticleData() const;
  void UnlockParticleData();

  void Render(iGraphics *renderer, csRenderPass pass, csMaterial *material);


private:
  iVertexDeclaration *m_vertexDeclaration;
  iVertexBuffer *m_buffer;

  csSize m_numParticles;
  csSize m_numRenderParticles;
  ParticleData *m_particleData;
};




