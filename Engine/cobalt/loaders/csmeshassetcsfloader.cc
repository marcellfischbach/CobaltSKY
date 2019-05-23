

#include <cobalt/loaders/csmeshassetcsfloader.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/iindexbuffer.hh>

#include <cobalt/csengine.hh>
#include <algorithm>


struct MaterialSlot
{
  std::string name;
  csSize idx;
};

namespace
{
bool slot_sort(MaterialSlot &slot0, MaterialSlot &slot1)
{
  return slot0.idx < slot1.idx;
}
}

cs::MeshAssetCSFLoader::MeshAssetCSFLoader()
  : cs::BaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::MeshAssetCSFLoader::~MeshAssetCSFLoader()
{

}



bool cs::MeshAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("mesh");
}

const cs::Class *cs::MeshAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::MeshWrapper::GetStaticClass();
}

cs::ResourceWrapper *cs::MeshAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::Mesh *mesh = new cs::Mesh();
  cs::MeshWrapper *meshWrapper = new cs::MeshWrapper(mesh);

  const cs::file::Entry *materialSlotsEntry = entry->GetEntry("materialSlots");
  const cs::file::Entry *globalIndicesEntry = entry->GetEntry("globalIndices");
  const cs::file::Entry *subMeshesEntry = entry->GetEntry("subMeshes");

  if (!materialSlotsEntry || !globalIndicesEntry || !subMeshesEntry)
  {
    return meshWrapper;
  }


  //
  // load the material slots
  std::vector<MaterialSlot> materialSlots;
  for (const cs::file::Entry *materialSlotEntry = materialSlotsEntry->GetEntry("materialSlot");
       materialSlotEntry;
       materialSlotEntry = materialSlotEntry->GetSiblingEntry("materialSlot"))
  {
    if (!materialSlotEntry->HasAttribute("name") || !materialSlotEntry->HasAttribute("id"))
    {
      continue;
    }
    MaterialSlot slot;
    slot.name = materialSlotEntry->GetAttribute("name");
    slot.idx = materialSlotEntry->GetAttributeInt("id");
    materialSlots.push_back(slot);
  }
  std::sort(materialSlots.begin(), materialSlots.end(), ::slot_sort);
  for (MaterialSlot &slot : materialSlots)
  {
    mesh->AddMaterialName(slot.name);
  }


  //
  // currently we don't support global index buffers


  //
  // lod the submeshes
  for (const cs::file::Entry *subMeshEntry = subMeshesEntry->GetEntry("subMesh");
       subMeshEntry;
       subMeshEntry = subMeshEntry->GetSiblingEntry("subMesh"))
  {
    std::string name;
    csSize materialSlot = 0;
    csSize lod = 0;
    if (subMeshEntry->HasAttribute("name"))
    {
      name = subMeshEntry->GetAttribute("name");
    }
    if (subMeshEntry->HasAttribute("materialSlot"))
    {
      materialSlot = subMeshEntry->GetAttributeInt("materialSlot");
    }
    if (subMeshEntry->HasAttribute("lod"))
    {
      lod = subMeshEntry->GetAttributeInt("lod");
    }

    cs::SubMeshWrapper *subMesh = nullptr;
    std::string blobName = subMeshEntry->GetAttribute();
    const cs::file::Blob *blob = subMeshEntry->GetFile()->GetBlob(blobName);
    if (blob)
    {
      subMesh = cs::ResourceManager::Get()->Load<cs::SubMeshWrapper>(blob, locator, userData);
    }
    if (subMesh)
    {
      mesh->AddMesh(subMesh, materialSlot, (csUInt8)lod, name);
      subMesh->Release();
    }
  }

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  return meshWrapper;
}















