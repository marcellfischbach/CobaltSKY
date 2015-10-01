

#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>

vkMesh::vkMesh()
  : vkObject()
  , m_vertexDeclaration(0)
  , m_indexType(eDT_UnsignedShort)
  , m_primitiveType(ePT_Triangles)
{

}

vkMesh::~vkMesh()
{
  if (m_vertexDeclaration)
  {
    m_vertexDeclaration->Release();
  }

  for (Index& index : m_indices)
  {
    if (index.m_indexBuffer)
    {
      index.m_indexBuffer->Release();
    }
  }

  for (IVertexBuffer *vertexBuffer : m_vertexBuffer)
  {
    vertexBuffer->Release();
  }
}

void vkMesh::SetPrimitiveType(vkPrimitiveType type)
{
  m_primitiveType = type;
}

void vkMesh::SetIndexType(vkDataType indexType)
{
  m_indexType = indexType;
}

void vkMesh::SetVertexDeclaration(IVertexDeclaration *vertexDeclaration)
{
  VK_SET(m_vertexDeclaration, vertexDeclaration);
}

void vkMesh::AddVertexBuffer(IVertexBuffer *vertexBuffer)
{
  if (vertexBuffer)
  {
    vertexBuffer->AddRef();
  }
  m_vertexBuffer.push_back(vertexBuffer);
}

void vkMesh::AddIndexBuffer(IIndexBuffer *indexBuffer, vkSize count, vkSize offset)
{
  if (!indexBuffer)
  {
    return;
  }
  indexBuffer->AddRef();

  Index index;
  index.m_indexBuffer =  indexBuffer;
  index.m_count = count;
  index.m_offset = offset;
  m_indices.push_back(index);
}


void vkMesh::Render(IRenderer *renderer, vkUInt8 lod)
{
  if (lod >= m_indices.size())
  {
    return;
  }

  Index &index = m_indices[lod];
  renderer->SetIndexBuffer(index.m_indexBuffer);
  for (vkSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer(i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, index.m_count, m_indexType);
}
