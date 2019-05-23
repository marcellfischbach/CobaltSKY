
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>


cs::SubMesh::SubMesh()
  : cs::Object()
  , m_vertexDeclaration(0)
  , m_indexType(cs::eDT_UnsignedShort)
  , m_primitiveType(cs::ePT_Triangles)
  , m_indexBuffer(0)
{
  m_boundingBox.Clear();
}

cs::SubMesh::~SubMesh()
{
  if (m_vertexDeclaration)
  {
    m_vertexDeclaration->Release();
  }

  if (m_indexBuffer)
  {
    m_indexBuffer->Release();
  }

  for (cs::iVertexBuffer *vertexBuffer : m_vertexBuffer)
  {
    vertexBuffer->Release();
  }
}

void cs::SubMesh::SetPrimitiveType(cs::ePrimitiveType type)
{
  m_primitiveType = type;
}

void cs::SubMesh::SetIndexType(cs::eDataType indexType)
{
  m_indexType = indexType;
}

void cs::SubMesh::SetVertexDeclaration(cs::iVertexDeclaration *vertexDeclaration)
{
  CS_SET(m_vertexDeclaration, vertexDeclaration);
}

void cs::SubMesh::AddVertexBuffer(cs::iVertexBuffer *vertexBuffer)
{
  if (vertexBuffer)
  {
    vertexBuffer->AddRef();
  }
  m_vertexBuffer.push_back(vertexBuffer);
}

void cs::SubMesh::SetIndexBuffer(cs::iIndexBuffer *indexBuffer, csSize count, csSize offset)
{
  CS_SET(m_indexBuffer, indexBuffer);

  m_count = count;
  m_offset = offset;
}

void cs::SubMesh::SetBoundingBox(const cs::BoundingBox &bbox)
{
  m_boundingBox = bbox;
}

const cs::BoundingBox &cs::SubMesh::GetBoundingBox() const
{
  return m_boundingBox;
}


void cs::SubMesh::Render(cs::iGraphics *renderer)
{
  renderer->SetIndexBuffer(m_indexBuffer);
  for (csSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer((csUInt16)i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, (csUInt32)m_count, m_indexType);
}

unsigned cs::SubMesh::GetNumberOfTrigons() const
{
  if (m_primitiveType == cs::ePT_Triangles)
  {
    return m_count / 3;
  }
  return m_count / 3;
}