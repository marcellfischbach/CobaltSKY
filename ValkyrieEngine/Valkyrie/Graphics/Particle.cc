

#include <Valkyrie/Graphics/Particle.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/Material.hh>

static IVertexDeclaration* create_particle_vertex_declaration(IGraphics *graphics)
{
  static IVertexDeclaration *result = 0;
  if (!result)
  {

    vkVertexElement elements[] = {
      vkVertexElement(eVST_Position, eDT_Float, 4, 0, sizeof(float) * 4, 0),
      vkVertexElement(eVST_ParticleSize, eDT_Float, 4, 0, sizeof(float) * 4, 1),
      vkVertexElement(eVST_ParticleRotation, eDT_Float, 3, 0, sizeof(float) * 3, 2),
      vkVertexElement(eVST_ParticleTexCoord, eDT_Float, 4, 0, sizeof(float) * 4, 3),
      vkVertexElement()
    };

    result = graphics->CreateVertexDeclaration(elements);
  }

  return result;
}


vkParticle::vkParticle()
  : vkObject()
  , m_positions(0)
  , m_sizes(0)
  , m_rotations(0)
  , m_texCoords(0)
  , m_vertexDeclaration(0)
  , m_numParticles(0)
  , m_numRenderParticles(0)
{
}

vkParticle::~vkParticle()
{
  VK_RELEASE(m_positions);
  VK_RELEASE(m_sizes);
  VK_RELEASE(m_rotations);
  VK_RELEASE(m_texCoords);


  // the vertex declaration is a static shared... this will NEVER be released
  m_vertexDeclaration = 0;
}

bool vkParticle::Initialize(IGraphics *graphics, vkSize numPartilces)
{
  SetNumberOfParticles(numPartilces);

  m_positions = graphics->CreateVertexBuffer(numPartilces * 4 * sizeof(float), 0, eBDM_Static);
  m_sizes = graphics->CreateVertexBuffer(numPartilces * 4 * sizeof(float), 0, eBDM_Static);
  m_rotations = graphics->CreateVertexBuffer(numPartilces * 3 * sizeof(float), 0, eBDM_Static);
  m_texCoords = graphics->CreateVertexBuffer(numPartilces * 4 * sizeof(float), 0, eBDM_Static);

  m_vertexDeclaration = create_particle_vertex_declaration(graphics);


  return true;
}


void vkParticle::SetPosition(IVertexBuffer *buffer)
{
  VK_SET(m_positions, buffer);
}

IVertexBuffer *vkParticle::GetPositions()
{
  return m_positions;
}

const IVertexBuffer *vkParticle::GetPositions() const
{
  return m_positions;
}

void vkParticle::SetSizes(IVertexBuffer *buffer)
{
  VK_SET(m_sizes, buffer);
}

IVertexBuffer *vkParticle::GetSizes()
{
  return m_sizes;
}

const IVertexBuffer *vkParticle::GetSizes() const
{
  return m_sizes;
}


void vkParticle::SetRotations(IVertexBuffer *buffer)
{
  VK_SET(m_rotations, buffer);
}

IVertexBuffer *vkParticle::GetRotations()
{
  return m_rotations;
}

const IVertexBuffer *vkParticle::GetRotations() const
{
  return m_rotations;
}


void vkParticle::SetTexCoords(IVertexBuffer *buffer)
{
  VK_SET(m_texCoords, buffer);
}

IVertexBuffer *vkParticle::GetTexCoords()
{
  return m_texCoords;
}

const IVertexBuffer *vkParticle::GetTexCoords() const
{
  return m_texCoords;
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

void vkParticle::Render(IGraphics *renderer, vkRenderPass pass, vkMaterialInstance *material)
{
  if (material->Bind(renderer, pass))
  {
    renderer->SetVertexBuffer(0, m_positions);
    renderer->SetVertexBuffer(1, m_sizes);
    renderer->SetVertexBuffer(2, m_rotations);
    renderer->SetVertexBuffer(3, m_texCoords);
    renderer->SetVertexDeclaration(m_vertexDeclaration);

    renderer->Render(ePT_Points, (vkUInt32)m_numRenderParticles);
  }
}

