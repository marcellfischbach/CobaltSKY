
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>


csSubMesh::csSubMesh()
  : cs::Object()
  , m_vertexDeclaration(0)
  , m_indexType(eDT_UnsignedShort)
  , m_primitiveType(ePT_Triangles)
  , m_indexBuffer(0)
{
  m_boundingBox.Clear();
}

csSubMesh::~csSubMesh()
{
  if (m_vertexDeclaration)
  {
    m_vertexDeclaration->Release();
  }

  if (m_indexBuffer)
  {
    m_indexBuffer->Release();
  }

  for (iVertexBuffer *vertexBuffer : m_vertexBuffer)
  {
    vertexBuffer->Release();
  }
}

void csSubMesh::SetPrimitiveType(csPrimitiveType type)
{
  m_primitiveType = type;
}

void csSubMesh::SetIndexType(csDataType indexType)
{
  m_indexType = indexType;
}

void csSubMesh::SetVertexDeclaration(iVertexDeclaration *vertexDeclaration)
{
  CS_SET(m_vertexDeclaration, vertexDeclaration);
}

void csSubMesh::AddVertexBuffer(iVertexBuffer *vertexBuffer)
{
  if (vertexBuffer)
  {
    vertexBuffer->AddRef();
  }
  m_vertexBuffer.push_back(vertexBuffer);
}

void csSubMesh::SetIndexBuffer(iIndexBuffer *indexBuffer, csSize count, csSize offset)
{
  CS_SET(m_indexBuffer, indexBuffer);

  m_count = count;
  m_offset = offset;
}

void csSubMesh::SetBoundingBox(const csBoundingBox &bbox)
{
  m_boundingBox = bbox;
}

const csBoundingBox &csSubMesh::GetBoundingBox() const
{
  return m_boundingBox;
}


void csSubMesh::Render(iGraphics *renderer)
{
  renderer->SetIndexBuffer(m_indexBuffer);
  for (csSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer((csUInt16)i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, (csUInt32)m_count, m_indexType);
}

unsigned csSubMesh::GetNumberOfTrigons() const
{
  if (m_primitiveType == ePT_Triangles)
  {
    return m_count / 3;
  }
  return m_count / 3;
}