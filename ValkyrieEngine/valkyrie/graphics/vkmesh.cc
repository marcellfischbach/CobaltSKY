

#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vkmaterialinstance.hh>
#include <valkyrie/graphics/vksubmesh.hh>
#include <algorithm>



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

const vkString &vkMesh::GetMaterialName(vkSize idx) const
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
  for (vkSize i = 0, in = m_materialNames.size(); i < in; ++i)
  {
    if (m_materialNames[i] == materialName)
    {
      return (vkUInt32)i;
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

void vkMesh::Render(IGraphics *renderer, vkRenderPass pass, vkSize numberOfMaterials, vkMaterialInstance **materials, vkUInt8 lodIdx)
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
        if (!nextMaterial->Bind(renderer, pass))
        {
          continue;
        }
        activeInstance = nextMaterial;
      }
      data.m_mesh->Render(renderer);
    }
  }

}
