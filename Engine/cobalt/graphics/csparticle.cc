

#include <cobalt/graphics/csparticle.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <stddef.h>

static iVertexDeclaration* create_particle_vertex_declaration(iGraphics *graphics)
{
  static iVertexDeclaration *result = 0;
  if (!result)
  {

    csVertexElement elements[] = {
      csVertexElement(eVST_Position, eDT_Float, 3, offsetof(csParticle::ParticleData, position), sizeof(csParticle::ParticleData), 0),
      csVertexElement(eVST_ParticleSize, eDT_Float, 2, offsetof(csParticle::ParticleData, size), sizeof(csParticle::ParticleData), 0),
      csVertexElement(eVST_ParticleRotation, eDT_Float, 1, offsetof(csParticle::ParticleData, rotation), sizeof(csParticle::ParticleData), 0),
      csVertexElement(eVST_ParticleTimeToLive, eDT_Float, 1, offsetof(csParticle::ParticleData, timeToLive), sizeof(csParticle::ParticleData), 0),
      csVertexElement(eVST_ParticleTextPage, eDT_Float, 3, offsetof(csParticle::ParticleData, textPage), sizeof(csParticle::ParticleData), 0),
      csVertexElement()
    };

    result = graphics->CreateVertexDeclaration(elements);
  }

  return result;
}


csParticle::csParticle()
  : cs::Object()
  , m_buffer(0)
  , m_vertexDeclaration(0)
  , m_numParticles(0)
  , m_numRenderParticles(0)
  , m_particleData(0)
{
}

csParticle::~csParticle()
{
  CS_RELEASE(m_buffer);


  // the vertex declaration is a static shared... this will NEVER be released
  m_vertexDeclaration = 0;
}

bool csParticle::Initialize(iGraphics *graphics, csSize numPartilces)
{
  SetNumberOfParticles(numPartilces);

  m_buffer = graphics->CreateVertexBuffer(numPartilces * sizeof(ParticleData), 0, eBDM_Static);
  m_vertexDeclaration = create_particle_vertex_declaration(graphics);


  return true;
}

void csParticle::SetParticleData(unsigned numParticles, const csParticle::ParticleData *data)
{
  if (m_buffer)
  {
    m_buffer->Copy(0, numParticles * sizeof(ParticleData), data);
  }
}

iVertexBuffer *csParticle::GetParticleBuffer()
{
  return m_buffer;
}

const iVertexBuffer *csParticle::GetParticleBuffer() const
{
  return m_buffer;
}

void csParticle::SetNumberOfParticles(csSize numParticles)
{
  m_numParticles = numParticles;
  if (m_numParticles < m_numRenderParticles)
  {
    SetNumberOfRenderParticles(numParticles);
  }
}

csSize csParticle::GetNumberOfParticles() const
{
  return m_numParticles;
}

void csParticle::SetNumberOfRenderParticles(csSize numRenderParticles)
{
  m_numRenderParticles = numRenderParticles;
}

csSize csParticle::GetNumberOfRenderParticles() const
{
  return m_numRenderParticles;
}



void csParticle::Render(iGraphics *renderer, csRenderPass pass, csMaterial *material)
{
  if (material->Bind(renderer, pass))
  {
    renderer->SetVertexBuffer(0, m_buffer);
    renderer->SetVertexDeclaration(m_vertexDeclaration);

    renderer->Render(ePT_Points, (csUInt32)m_numRenderParticles);
  }
}


bool csParticle::LockParticleData(bool readOnly)
{
  m_particleData = 0;
  if (m_buffer->Lock(0, (void**)&m_particleData, readOnly ? eBAM_Read : eBAM_ReadWrite))
  {
    return true;
  }
  return false;
}

csParticle::ParticleData *csParticle::GetParticleData()
{
  return m_particleData;
}

const csParticle::ParticleData *csParticle::GetParticleData() const
{
  return m_particleData;
}

void csParticle::UnlockParticleData()
{
  if (m_particleData)
  {
    m_buffer->Unlock();
    m_particleData = 0;
  }
}

