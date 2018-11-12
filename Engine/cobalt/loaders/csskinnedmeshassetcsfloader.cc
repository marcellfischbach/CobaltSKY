

#include <cobalt/loaders/csskinnedmeshassetcsfloader.hh>
#include <cobalt/graphics/csskinnedmesh.hh>
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

csSkinnedMeshAssetCSFLoader::csSkinnedMeshAssetCSFLoader()
  : csBaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csSkinnedMeshAssetCSFLoader::~csSkinnedMeshAssetCSFLoader()
{

}



bool csSkinnedMeshAssetCSFLoader::CanLoad(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return entry->GetTagName() == std::string("skinnedMesh");
}

const csClass *csSkinnedMeshAssetCSFLoader::EvalClass(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  return csSkinnedMesh::GetStaticClass();
}

csResourceWrapper *csSkinnedMeshAssetCSFLoader::Load(const csfEntry *entry, const csResourceLocator &locator, iObject *userData) const
{
  csSkinnedMesh *mesh = new csSkinnedMesh();
  csSkinnedMeshWrapper *meshWrapper = new csSkinnedMeshWrapper(mesh);

  const csfEntry *materialSlotsEntry = entry->GetEntry("materialSlots");
  const csfEntry *globalIndicesEntry = entry->GetEntry("globalIndices");
  const csfEntry *subMeshesEntry = entry->GetEntry("subMeshes");
  const csfEntry *bonesEntry = entry->GetEntry("bones");

  if (!materialSlotsEntry || !globalIndicesEntry || !subMeshesEntry || !bonesEntry)
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


  for (const csfEntry *boneEntry = bonesEntry->GetEntry("bone");
       boneEntry;
       boneEntry = boneEntry->GetSiblingEntry("bone"))
  {
    if (boneEntry->HasAttribute("id") && boneEntry->HasAttribute("name"))
    {
      std::string name = boneEntry->GetAttribute("name");
      csUInt8 id = (csUInt8)boneEntry->GetAttributeInt("id");
      mesh->AddBoneName(name, id);
    }
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

    if (HasLocator(subMeshEntry))
    {
      csSubMeshWrapper *subMesh = csResourceManager::Get()->GetOrLoad<csSubMeshWrapper>(GetLocator(subMeshEntry));
      if (subMesh)
      {
        mesh->AddMesh(subMesh, materialSlot, (csUInt8)lod, name);
        subMesh->Release();
      }
    }
  }

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  return meshWrapper;
}



