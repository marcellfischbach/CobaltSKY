

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

csMeshAssetCSFLoader::csMeshAssetCSFLoader()
  : csBaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csMeshAssetCSFLoader::~csMeshAssetCSFLoader()
{

}



bool csMeshAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("mesh");
}

const cs::Class *csMeshAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  return csMeshWrapper::GetStaticClass();
}

csResourceWrapper *csMeshAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, cs::iObject *userData) const
{
  csMesh *mesh = new csMesh();
  csMeshWrapper *meshWrapper = new csMeshWrapper(mesh);

  const csfEntry *materialSlotsEntry = entry->GetEntry("materialSlots");
  const csfEntry *globalIndicesEntry = entry->GetEntry("globalIndices");
  const csfEntry *subMeshesEntry = entry->GetEntry("subMeshes");

  if (!materialSlotsEntry || !globalIndicesEntry || !subMeshesEntry)
  {
    return meshWrapper;
  }


  //
  // load the material slots
  std::vector<MaterialSlot> materialSlots;
  for (const csfEntry *materialSlotEntry = materialSlotsEntry->GetEntry("materialSlot");
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
  for (const csfEntry *subMeshEntry = subMeshesEntry->GetEntry("subMesh");
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

    csSubMeshWrapper *subMesh = nullptr;
    std::string blobName = subMeshEntry->GetAttribute();
    const csfBlob *blob = subMeshEntry->GetFile()->GetBlob(blobName);
    if (blob)
    {
      subMesh = csResourceManager::Get()->Load<csSubMeshWrapper>(blob, locator, userData);
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















