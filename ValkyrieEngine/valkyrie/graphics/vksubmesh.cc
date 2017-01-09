
#include <valkyrie/graphics/vksubmesh.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/iindexbuffer.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>


vkSubMesh::vkSubMesh()
  : vkObject()
  , m_vertexDeclaration(0)
  , m_indexType(eDT_UnsignedShort)
  , m_primitiveType(ePT_Triangles)
  , m_indexBuffer(0)
{
  m_boundingBox.Clear();
}

vkSubMesh::~vkSubMesh()
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

void vkSubMesh::SetPrimitiveType(vkPrimitiveType type)
{
  m_primitiveType = type;
}

void vkSubMesh::SetIndexType(vkDataType indexType)
{
  m_indexType = indexType;
}

void vkSubMesh::SetVertexDeclaration(iVertexDeclaration *vertexDeclaration)
{
  VK_SET(m_vertexDeclaration, vertexDeclaration);
}

void vkSubMesh::AddVertexBuffer(iVertexBuffer *vertexBuffer)
{
  if (vertexBuffer)
  {
    vertexBuffer->AddRef();
  }
  m_vertexBuffer.push_back(vertexBuffer);
}

void vkSubMesh::SetIndexBuffer(iIndexBuffer *indexBuffer, vkSize count, vkSize offset)
{
  VK_SET(m_indexBuffer, indexBuffer);

  m_count = count;
  m_offset = offset;
}

void vkSubMesh::SetBoundingBox(const vkBoundingBox &bbox)
{
  m_boundingBox = bbox;
}

const vkBoundingBox &vkSubMesh::GetBoundingBox() const
{
  return m_boundingBox;
}


void vkSubMesh::Render(iGraphics *renderer)
{
  renderer->SetIndexBuffer(m_indexBuffer);
  for (vkSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer((vkUInt16)i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, (vkUInt32)m_count, m_indexType);
}

