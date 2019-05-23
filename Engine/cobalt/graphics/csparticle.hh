#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/csparticle.refl.hh>



namespace cs
{
class Material;
class Particle;
struct iGraphics;
struct iVertexBuffer;
struct iVertexDeclaration;


CS_CLASS()
class CSE_API Particle : public CS_SUPER(cs::Object)
{
  CS_CLASS_GEN;

public:
  struct ParticleData
  {
    cs::Vector3f position;
    cs::Vector3f direction;
    cs::Vector2f size;
    cs::Vector4f sizeRange;
    float      sizeTime;
    float      rotation;
    float      rotationSpeed;
    float      time;
    float      timeToLive;
    cs::Vector3f textPage;
  };

public:
  Particle();
  virtual ~Particle();

  bool Initialize(cs::iGraphics *renderer, csSize numberOfParticles);

  void SetParticleData(unsigned numParticles, const ParticleData *data);
  cs::iVertexBuffer *GetParticleBuffer();
  const cs::iVertexBuffer *GetParticleBuffer() const;

  void SetNumberOfParticles(csSize numParticles);
  csSize GetNumberOfParticles() const;

  void SetNumberOfRenderParticles(csSize numParticles);
  csSize GetNumberOfRenderParticles() const;

  bool LockParticleData(bool readOnly = false);
  ParticleData *GetParticleData();
  const ParticleData *GetParticleData() const;
  void UnlockParticleData();

  void Render(cs::iGraphics *renderer, cs::eRenderPass pass, cs::Material *material);


private:
  cs::iVertexDeclaration *m_vertexDeclaration;
  cs::iVertexBuffer *m_buffer;

  csSize m_numParticles;
  csSize m_numRenderParticles;
  ParticleData *m_particleData;
};




}
