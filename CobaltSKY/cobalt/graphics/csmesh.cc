

#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <algorithm>



csMesh::csMesh()
  : csObject()
  , m_numberOfMaterials(0)
{

}

csMesh::~csMesh()
{
  for (csSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];
    for (csSize j = 0, jn = lod.m_meshes.size(); j < jn; ++j)
    {
      lod.m_meshes[j].m_mesh->Release();
    }
    lod.m_meshes.clear();
  }
  m_lods.clear();
}



void csMesh::AddMesh(csSubMesh *mesh, csSize materialIndex, csUInt8 lodIdx, const csString &name)
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

void csMesh::AddMaterialName(const csString &materialName)
{
  m_materialNames.push_back(materialName);
}

const csString &csMesh::GetMaterialName(csSize idx) const
{
  if (idx >= m_materialNames.size())
  {
    static csString undef("<undef>");
    return undef;
  }

  return m_materialNames[idx];
}

csUInt32 csMesh::GetMaterialIndex(const csString &materialName) const
{
  for (csSize i = 0, in = m_materialNames.size(); i < in; ++i)
  {
    if (m_materialNames[i] == materialName)
    {
      return (csUInt32)i;
    }
  }
  return ~0x00;
}


void csMesh::OptimizeDataStruct()
{
  for (csSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];

    std::sort(lod.m_meshes.begin(), lod.m_meshes.end(), [](Data &a, Data &b) {
      return a.m_materialIndex < b.m_materialIndex;
    });
  }
}

void csMesh::UpdateBoundingBox()
{
  m_boundingBox.Finish();
}

csUInt8 csMesh::GetNumberOfLODs() const
{
  return (csUInt8)m_lods.size();
}

csSize csMesh::GetNumberOfMeshes(csUInt8 lod) const
{
  return m_lods[lod].m_meshes.size();
}

csSubMesh *csMesh::GetMesh(csUInt8 lod, csSize idx)
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

const csSubMesh *csMesh::GetMesh(csUInt8 lod, csSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

csSize csMesh::GetMaterialIndex(csUInt8 lod, csSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_materialIndex;
}


csMesh::LOD &csMesh::GetLOD(csUInt8 lod)
{
  if (m_lods.size() <= lod)
  {
    csUInt8 missing = lod - (csUInt8)m_lods.size() + 1;
    for (csUInt8 i = 0; i < missing; ++i)
    {
      LOD lod;
      lod.m_meshes.clear();
      m_lods.push_back(lod);
    }
  }
  return m_lods[lod];
}

void csMesh::Render(iGraphics *renderer, csRenderPass pass, csSize numberOfMaterials, csMaterial **materials, csUInt8 lodIdx)
{
  if (lodIdx >= m_lods.size())
  {
    return;
  }

  csMaterial *activeInstance = 0;

  LOD &lod = m_lods[lodIdx];
  for (csSize i = 0, in = lod.m_meshes.size(); i < in; ++i)
  {
    Data &data = lod.m_meshes[i];
    if (data.m_mesh)
    {
      
      
      if (data.m_materialIndex >= numberOfMaterials)
      {
        continue;
      }
      csMaterial *nextMaterial = materials[data.m_materialIndex];
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