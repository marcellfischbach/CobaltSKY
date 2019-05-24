

#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <algorithm>



cs::Mesh::Mesh()
  : cs::iMesh()
  , m_numberOfMaterials(0)
{
  CS_CLASS_GEN_CONSTR;
}

cs::Mesh::~Mesh()
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



void cs::Mesh::AddMesh(cs::SubMeshWrapper *mesh, csSize materialIndex, csUInt8 lodIdx, const std::string &name)
{
  if (mesh && mesh->IsValid())
  {
    LOD &lod = GetLOD(lodIdx);

    mesh->AddRef();
    Data data;
    data.m_mesh = mesh;
    data.m_materialIndex = materialIndex;
    data.m_name = name;
    lod.m_meshes.push_back(data);

    m_boundingBox.Add(mesh->Get()->GetBoundingBox());

    if (materialIndex >= m_numberOfMaterials)
    {
      m_numberOfMaterials = materialIndex + 1;
    }
  }
}

void cs::Mesh::AddMaterialName(const std::string &materialName)
{
  m_materialNames.push_back(materialName);
}

const std::string &cs::Mesh::GetMaterialName(csSize idx) const
{
  if (idx >= m_materialNames.size())
  {
    static std::string undef("<undef>");
    return undef;
  }

  return m_materialNames[idx];
}

csUInt32 cs::Mesh::GetMaterialIndex(const std::string &materialName) const
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


void cs::Mesh::OptimizeDataStruct()
{
  for (csSize i = 0, in = m_lods.size(); i < in; ++i)
  {
    LOD &lod = m_lods[i];

    std::sort(lod.m_meshes.begin(), lod.m_meshes.end(), [](Data &a, Data &b) {
      return a.m_materialIndex < b.m_materialIndex;
    });
  }
}

void cs::Mesh::UpdateBoundingBox()
{
  m_boundingBox.Finish();
}

csUInt8 cs::Mesh::GetNumberOfLODs() const
{
  return (csUInt8)m_lods.size();
}

csSize cs::Mesh::GetNumberOfMeshes(csUInt8 lod) const
{
  return m_lods[lod].m_meshes.size();
}

cs::SubMeshWrapper *cs::Mesh::GetMesh(csUInt8 lod, csSize idx)
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

const cs::SubMeshWrapper *cs::Mesh::GetMesh(csUInt8 lod, csSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_mesh;
}

csSize cs::Mesh::GetMaterialIndex(csUInt8 lod, csSize idx) const
{
  return m_lods[lod].m_meshes[idx].m_materialIndex;
}


cs::Mesh::LOD &cs::Mesh::GetLOD(csUInt8 lod)
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

void cs::Mesh::Update(cs::iGraphics *renderer, const cs::Vector3f &cameraPos, csUInt64 frameNo)
{
  // no update here
}

void cs::Mesh::Render(cs::iGraphics *renderer, cs::eRenderPass pass, const std::vector<cs::Material *> &materials, csUInt8 lodIdx)
{
  if (lodIdx >= m_lods.size())
  {
    return;
  }

  cs::Material *activeInstance = 0;

  LOD &lod = m_lods[lodIdx];
  for (csSize i = 0, in = lod.m_meshes.size(); i < in; ++i)
  {
    Data &data = lod.m_meshes[i];
    if (data.m_mesh)
    {
      
      
      if (data.m_materialIndex >= materials.size())
      {
        continue;
      }
      cs::Material *nextMaterial = materials[data.m_materialIndex];
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
      data.m_mesh->Get()->Render(renderer);
    }
  }

}


csSize cs::Mesh::GetNumberOfRenderCalls(csUInt8 lodIdx) const
{
  const LOD &lod = m_lods[lodIdx];
  return lod.m_meshes.size();

}

csSize cs::Mesh::GetNumberOfTotalTrigons(csUInt8 lodIdx) const
{
  csSize count = 0;
  const LOD &lod = m_lods[lodIdx];
  for (auto data : lod.m_meshes)
  {
    const cs::SubMeshWrapper *subMesh = data.m_mesh;
    count += subMesh->Get()->GetNumberOfTrigons();
  }
  return count;

}

