

#include <valkyrie/graphics/vkparticle.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <valkyrie/graphics/vkmaterial.hh>
#include <stddef.h>

static iVertexDeclaration* create_particle_vertex_declaration(iGraphics *graphics)
{
  static iVertexDeclaration *result = 0;
  if (!result)
  {

    vkVertexElement elements[] = {
      vkVertexElement(eVST_Position, eDT_Float, 3, offsetof(vkParticle::ParticleData, position), sizeof(vkParticle::ParticleData), 0),
      vkVertexElement(eVST_ParticleSize, eDT_Float, 2, offsetof(vkParticle::ParticleData, size), sizeof(vkParticle::ParticleData), 0),
      vkVertexElement(eVST_ParticleRotation, eDT_Float, 1, offsetof(vkParticle::ParticleData, rotation), sizeof(vkParticle::ParticleData), 0),
      vkVertexElement(eVST_ParticleTimeToLive, eDT_Float, 1, offsetof(vkParticle::ParticleData, timeToLive), sizeof(vkParticle::ParticleData), 0),
      vkVertexElement(eVST_ParticleTextPage, eDT_Float, 3, offsetof(vkParticle::ParticleData, textPage), sizeof(vkParticle::ParticleData), 0),
      vkVertexElement()
    };

    result = graphics->CreateVertexDeclaration(elements);
  }

  return result;
}


vkParticle::vkParticle()
  : vkObject()
  , m_buffer(0)
  , m_vertexDeclaration(0)
  , m_numParticles(0)
  , m_numRenderParticles(0)
  , m_particleData(0)
{
}

vkParticle::~vkParticle()
{
  VK_RELEASE(m_buffer);


  // the vertex declaration is a static shared... this will NEVER be released
  m_vertexDeclaration = 0;
}

bool vkParticle::Initialize(iGraphics *graphics, vkSize numPartilces)
{
  SetNumberOfParticles(numPartilces);

  m_buffer = graphics->CreateVertexBuffer(numPartilces * sizeof(ParticleData), 0, eBDM_Static);
  m_vertexDeclaration = create_particle_vertex_declaration(graphics);


  return true;
}

void vkParticle::SetParticleData(unsigned numParticles, const vkParticle::ParticleData *data)
{
  if (m_buffer)
  {
    m_buffer->Copy(0, numParticles * sizeof(ParticleData), data);
  }
}

iVertexBuffer *vkParticle::GetParticleBuffer()
{
  return m_buffer;
}

const iVertexBuffer *vkParticle::GetParticleBuffer() const
{
  return m_buffer;
}

void vkParticle::SetNumberOfParticles(vkSize numParticles)
{
  m_numParticles = numParticles;
  if (m_numParticles < m_numRenderParticles)
  {
    SetNumberOfRenderParticles(numParticles);
  }
}

vkSize vkParticle::GetNumberOfParticles() const
{
  return m_numParticles;
}

void vkParticle::SetNumberOfRenderParticles(vkSize numRenderParticles)
{
  m_numRenderParticles = numRenderParticles;
}

vkSize vkParticle::GetNumberOfRenderParticles() const
{
  return m_numRenderParticles;
}



void vkParticle::Render(iGraphics *renderer, vkRenderPass pass, vkMaterial *material)
{
  if (material->Bind(renderer, pass))
  {
    renderer->SetVertexBuffer(0, m_buffer);
    renderer->SetVertexDeclaration(m_vertexDeclaration);

    renderer->Render(ePT_Points, (vkUInt32)m_numRenderParticles);
  }
}


bool vkParticle::LockParticleData(bool readOnly)
{
  m_particleData = 0;
  if (m_buffer->Lock(0, (void**)&m_particleData, readOnly ? eBAM_Read : eBAM_ReadWrite))
  {
    return true;
  }
  return false;
}

vkParticle::ParticleData *vkParticle::GetParticleData()
{
  return m_particleData;
}

const vkParticle::ParticleData *vkParticle::GetParticleData() const
{
  return m_particleData;
}

void vkParticle::UnlockParticleData()
{
  if (m_particleData)
  {
    m_buffer->Unlock();
    m_particleData = 0;
  }
}

