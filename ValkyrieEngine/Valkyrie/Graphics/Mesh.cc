

#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <algorithm>

vkMesh::vkMesh()
  : vkObject()
  , m_vertexDeclaration(0)
  , m_indexType(eDT_UnsignedShort)
  , m_primitiveType(ePT_Triangles)
  , m_indexBuffer(0)
{
  m_boundingBox.Clear();
}

vkMesh::~vkMesh()
{
  if (m_vertexDeclaration)
  {
    m_vertexDeclaration->Release();
  }

  if (m_indexBuffer)
  {
    m_indexBuffer->Release();
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

void vkMesh::SetIndexBuffer(IIndexBuffer *indexBuffer, vkSize count, vkSize offset)
{
  VK_SET(m_indexBuffer, indexBuffer);

  m_count = count;
  m_offset = offset;
}

void vkMesh::SetBoundingBox(const vkBoundingBox &bbox)
{
  m_boundingBox = bbox;
}

const vkBoundingBox &vkMesh::GetBoundingBox() const
{
  return m_boundingBox;
}


void vkMesh::Render(IGraphics *renderer)
{
  renderer->SetIndexBuffer(m_indexBuffer);
  for (vkSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer((vkUInt16)i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, (vkUInt32)m_count, m_indexType);
}




vkMultiMesh::vkMultiMesh()
  : vkObject()
{

}

vkMultiMesh::~vkMultiMesh()
{
  for (vkSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];
    for (vkSize j = 0, jn = lod.m_meshes.size(); j < jn; ++j)
    {
      lod.m_meshes[j].m_mesh->Release();
    }
    lod.m_meshes.clear();
  }
  m_lods.clear();
}



void vkMultiMesh::AddMesh(vkMesh *mesh, vkSize materialIndex, vkUInt8 lodIdx, const vkString &name)
{
  if (mesh)
  {
    LOD &lod = GetLOD(lodIdx);

    mesh->AddRef();
    Data data;
    data.m_mesh = mesh;
    data.m_materialIndex = materialIndex;
    data.m_name = name;
    lod.m_meshes.push_back(data);
  }
}

void vkMultiMesh::OptimizeDataStruct()
{
  for (vkSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];

    std::sort(lod.m_meshes.begin(), lod.m_meshes.end(), [](Data &a, Data &b) {
      return a.m_materialIndex < b.m_materialIndex;
    });
  }
}

vkUInt8 vkMultiMesh::GetNumberOfLODs() const
{
  return (vkUInt8)m_lods.size();
}

vkSize vkMultiMesh::GetNumberOfMeshes(vkUInt8 lod) const
{
  return m_lods[lod].m_meshes.size();
}

vkMesh *vkMultiMesh::GetMesh(vkUInt8 lod, vkSize idx)
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

const vkMesh *vkMultiMesh::GetMesh(vkUInt8 lod, vkSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

vkSize vkMultiMesh::GetMaterialIndex(vkUInt8 lod, vkSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_materialIndex;
}


vkMultiMesh::LOD &vkMultiMesh::GetLOD(vkUInt8 lod)
{
  if (m_lods.size() <= lod)
  {
    vkUInt8 missing = lod - (vkUInt8)m_lods.size() + 1;
    for (vkUInt8 i = 0; i < missing; ++i)
    {
      LOD lod;
      lod.m_meshes.clear();
      m_lods.push_back(lod);
    }
  }
  return m_lods[lod];
}

void vkMultiMesh::Render(IGraphics *renderer, vkRenderPass pass, vkMultiMaterial *material, vkUInt8 lodIdx)
{
  if (lodIdx >= m_lods.size())
  {
    return;
  }

  vkMaterialInstance *activeInstance = 0;

  LOD &lod = m_lods[lodIdx];
  for (vkSize i = 0, in = lod.m_meshes.size(); i < in; ++i)
  {
    Data &data = lod.m_meshes[i];
    if (data.m_mesh)
    {
      vkMaterialInstance *nextInstance = material->GetMaterialInstance(data.m_materialIndex);
      if (nextInstance == 0)
      {
        continue;
      }

      if (nextInstance != activeInstance)
      {
        activeInstance = nextInstance;
        if (!activeInstance->Bind(renderer, pass))
        {
          continue;
        }
      }

      data.m_mesh->Render(renderer);
    }
  }

}


