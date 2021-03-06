

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

cs::SkinnedMeshAssetCSFLoader::SkinnedMeshAssetCSFLoader()
  : cs::BaseCSFLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::SkinnedMeshAssetCSFLoader::~SkinnedMeshAssetCSFLoader()
{

}



bool cs::SkinnedMeshAssetCSFLoader::CanLoad(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return entry->GetTagName() == std::string("skinnedMesh");
}

const cs::Class *cs::SkinnedMeshAssetCSFLoader::EvalClass(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::SkinnedMesh::GetStaticClass();
}

cs::ResourceWrapper *cs::SkinnedMeshAssetCSFLoader::Load(const cs::file::Entry *entry, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::SkinnedMesh *mesh = new cs::SkinnedMesh();
  cs::SkinnedMeshWrapper *meshWrapper = new cs::SkinnedMeshWrapper(mesh);

  const cs::file::Entry *materialSlotsEntry = entry->GetEntry("materialSlots");
  const cs::file::Entry *globalIndicesEntry = entry->GetEntry("globalIndices");
  const cs::file::Entry *subMeshesEntry = entry->GetEntry("subMeshes");
  const cs::file::Entry *bonesEntry = entry->GetEntry("bones");

  if (!materialSlotsEntry || !globalIndicesEntry || !subMeshesEntry || !bonesEntry)
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


  for (const cs::file::Entry *boneEntry = bonesEntry->GetEntry("bone");
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

    if (HasLocator(subMeshEntry))
    {
      cs::SubMeshWrapper *subMesh = cs::ResourceManager::Get()->GetOrLoad<cs::SubMeshWrapper>(GetLocator(subMeshEntry));
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



