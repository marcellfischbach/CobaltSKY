

#include <cobalt/graphics/csparticle.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <stddef.h>

static cs::iVertexDeclaration* create_particle_vertex_declaration(cs::iGraphics *graphics)
{
  static cs::iVertexDeclaration *result = 0;
  if (!result)
  {

    cs::VertexElement elements[] = {
      cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 3, offsetof(cs::Particle::ParticleData, position), sizeof(cs::Particle::ParticleData), 0),
      cs::VertexElement(cs::eVST_ParticleSize, cs::eDT_Float, 2, offsetof(cs::Particle::ParticleData, size), sizeof(cs::Particle::ParticleData), 0),
      cs::VertexElement(cs::eVST_ParticleRotation, cs::eDT_Float, 1, offsetof(cs::Particle::ParticleData, rotation), sizeof(cs::Particle::ParticleData), 0),
      cs::VertexElement(cs::eVST_ParticleTimeToLive, cs::eDT_Float, 1, offsetof(cs::Particle::ParticleData, timeToLive), sizeof(cs::Particle::ParticleData), 0),
      cs::VertexElement(cs::eVST_ParticleTextPage, cs::eDT_Float, 3, offsetof(cs::Particle::ParticleData, textPage), sizeof(cs::Particle::ParticleData), 0),
      cs::VertexElement()
    };

    result = graphics->CreateVertexDeclaration(elements);
  }

  return result;
}


cs::Particle::Particle()
  : cs::Object()
  , m_buffer(0)
  , m_vertexDeclaration(0)
  , m_numParticles(0)
  , m_numRenderParticles(0)
  , m_particleData(0)
{
}

cs::Particle::~Particle()
{
  CS_RELEASE(m_buffer);


  // the vertex declaration is a static shared... this will NEVER be released
  m_vertexDeclaration = 0;
}

bool cs::Particle::Initialize(cs::iGraphics *graphics, csSize numPartilces)
{
  SetNumberOfParticles(numPartilces);

  m_buffer = graphics->CreateVertexBuffer(numPartilces * sizeof(ParticleData), 0, cs::eBDM_Static);
  m_vertexDeclaration = create_particle_vertex_declaration(graphics);


  return true;
}

void cs::Particle::SetParticleData(unsigned numParticles, const cs::Particle::ParticleData *data)
{
  if (m_buffer)
  {
    m_buffer->Copy(0, numParticles * sizeof(ParticleData), data);
  }
}

cs::iVertexBuffer *cs::Particle::GetParticleBuffer()
{
  return m_buffer;
}

const cs::iVertexBuffer *cs::Particle::GetParticleBuffer() const
{
  return m_buffer;
}

void cs::Particle::SetNumberOfParticles(csSize numParticles)
{
  m_numParticles = numParticles;
  if (m_numParticles < m_numRenderParticles)
  {
    SetNumberOfRenderParticles(numParticles);
  }
}

csSize cs::Particle::GetNumberOfParticles() const
{
  return m_numParticles;
}

void cs::Particle::SetNumberOfRenderParticles(csSize numRenderParticles)
{
  m_numRenderParticles = numRenderParticles;
}

csSize cs::Particle::GetNumberOfRenderParticles() const
{
  return m_numRenderParticles;
}



void cs::Particle::Render(cs::iGraphics *renderer, cs::eRenderPass pass, cs::Material *material)
{
  if (material->Bind(renderer, pass))
  {
    renderer->SetVertexBuffer(0, m_buffer);
    renderer->SetVertexDeclaration(m_vertexDeclaration);

    renderer->Render(cs::ePT_Points, (csUInt32)m_numRenderParticles);
  }
}


bool cs::Particle::LockParticleData(bool readOnly)
{
  m_particleData = 0;
  if (m_buffer->Lock(0, (void**)&m_particleData, readOnly ? cs::eBAM_Read : cs::eBAM_ReadWrite))
  {
    return true;
  }
  return false;
}

cs::Particle::ParticleData *cs::Particle::GetParticleData()
{
  return m_particleData;
}

const cs::Particle::ParticleData *cs::Particle::GetParticleData() const
{
  return m_particleData;
}

void cs::Particle::UnlockParticleData()
{
  if (m_particleData)
  {
    m_buffer->Unlock();
    m_particleData = 0;
  }
}

