

#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/Material.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <algorithm>

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

  for (IVertexBuffer *vertexBuffer : m_vertexBuffer)
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

void vkSubMesh::SetVertexDeclaration(IVertexDeclaration *vertexDeclaration)
{
  VK_SET(m_vertexDeclaration, vertexDeclaration);
}

void vkSubMesh::AddVertexBuffer(IVertexBuffer *vertexBuffer)
{
  if (vertexBuffer)
  {
    vertexBuffer->AddRef();
  }
  m_vertexBuffer.push_back(vertexBuffer);
}

void vkSubMesh::SetIndexBuffer(IIndexBuffer *indexBuffer, vkSize count, vkSize offset)
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


void vkSubMesh::Render(IGraphics *renderer)
{
  renderer->SetIndexBuffer(m_indexBuffer);
  for (vkSize i = 0, in = m_vertexBuffer.size(); i < in; ++i)
  {
    renderer->SetVertexBuffer((vkUInt16)i, m_vertexBuffer[i]);
  }
  renderer->SetVertexDeclaration(m_vertexDeclaration);
  renderer->RenderIndexed(m_primitiveType, (vkUInt32)m_count, m_indexType);
}




vkMesh::vkMesh()
  : vkObject()
  , m_numberOfMaterials(0)
{

}

vkMesh::~vkMesh()
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



void vkMesh::AddMesh(vkSubMesh *mesh, vkSize materialIndex, vkUInt8 lodIdx, const vkString &name)
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

    m_boundingBox.Add(mesh->GetBoundingBox());

    if (materialIndex >= m_numberOfMaterials)
    {
      m_numberOfMaterials = materialIndex + 1;
    }
  }
}

void vkMesh::AddMaterialName(const vkString &materialName)
{
  m_materialNames.push_back(materialName);
}

const vkString &vkMesh::GetMaterialName(vkUInt32 idx) const
{
  if (idx >= m_materialNames.size())
  {
    static vkString undef("<undef>");
    return undef;
  }

  return m_materialNames[idx];
}

vkUInt32 vkMesh::GetMaterialIndex(const vkString &materialName) const
{
  for (vkUInt32 i = 0, in = m_materialNames.size(); i < in; ++i)
  {
    if (m_materialNames[i] == materialName)
    {
      return i;
    }
  }
  return ~0x00;
}


void vkMesh::OptimizeDataStruct()
{
  for (vkSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];

    std::sort(lod.m_meshes.begin(), lod.m_meshes.end(), [](Data &a, Data &b) {
      return a.m_materialIndex < b.m_materialIndex;
    });
  }
}

void vkMesh::UpdateBoundingBox()
{
  m_boundingBox.Finish();
}

vkUInt8 vkMesh::GetNumberOfLODs() const
{
  return (vkUInt8)m_lods.size();
}

vkSize vkMesh::GetNumberOfMeshes(vkUInt8 lod) const
{
  return m_lods[lod].m_meshes.size();
}

vkSubMesh *vkMesh::GetMesh(vkUInt8 lod, vkSize idx)
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

const vkSubMesh *vkMesh::GetMesh(vkUInt8 lod, vkSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

vkSize vkMesh::GetMaterialIndex(vkUInt8 lod, vkSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_materialIndex;
}


vkMesh::LOD &vkMesh::GetLOD(vkUInt8 lod)
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

void vkMesh::Render(IGraphics *renderer, vkRenderPass pass, vkUInt32 numberOfMaterials, vkMaterialInstance **materials, vkUInt8 lodIdx)
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
      
      
      if (data.m_materialIndex >= numberOfMaterials)
      {
        continue;
      }
      vkMaterialInstance *nextMaterial = materials[data.m_materialIndex];
      if (nextMaterial == 0)
      {
        continue;
      }

      if (nextMaterial != activeInstance)
      {
        activeInstance = nextMaterial;
        if (!activeInstance->Bind(renderer, pass))
        {
          continue;
        }
      }

      data.m_mesh->Render(renderer);
    }
  }

}


