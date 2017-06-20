

#include <cobalt/loaders/csskinnedmeshassetxmlloader.hh>
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
  csString name;
  csSize idx;
};

namespace
{
bool slot_sort(MaterialSlot &slot0, MaterialSlot &slot1)
{
  return slot0.idx < slot1.idx;
}
}

csSkinnedMeshAssetXMLLoader::csSkinnedMeshAssetXMLLoader()
  : csBaseXMLLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csSkinnedMeshAssetXMLLoader::~csSkinnedMeshAssetXMLLoader()
{

}



bool csSkinnedMeshAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csString(element->Value()) == csString("skinnedMesh");
}

const csClass *csSkinnedMeshAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csSkinnedMesh::GetStaticClass();
}

iObject *csSkinnedMeshAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csSkinnedMesh *mesh = new csSkinnedMesh();

  TiXmlElement *materialSlotsElement = element->FirstChildElement("materialSlots");
  TiXmlElement *globalIndicesElement = element->FirstChildElement("globalIndices");
  TiXmlElement *subMeshesElement = element->FirstChildElement("subMeshes");
  TiXmlElement *bonesElement = element->FirstChildElement("bones");

  if (!materialSlotsElement || !globalIndicesElement || !subMeshesElement || !bonesElement)
  {
    return mesh;
  }


  // 
  // load the material slots
  std::vector<MaterialSlot> slots;
  for (TiXmlElement *materialSlotElement = materialSlotsElement->FirstChildElement("materialSlot");
       materialSlotElement;
       materialSlotElement = materialSlotElement->NextSiblingElement("materialSlot"))
  {
    if (!materialSlotElement->Attribute("name") || !materialSlotElement->Attribute("id"))
    {
      continue;
    }
    MaterialSlot slot;
    slot.name = csString(materialSlotElement->Attribute("name"));
    slot.idx = atoi(materialSlotElement->Attribute("id"));
    slots.push_back(slot);
  }
  std::sort(slots.begin(), slots.end(), ::slot_sort);
  for (MaterialSlot &slot : slots)
  {
    mesh->AddMaterialName(slot.name);
  }


  for (TiXmlElement *boneElement = bonesElement->FirstChildElement("bone");
       boneElement;
       boneElement = boneElement->NextSiblingElement("bone"))
  {
    if (boneElement->Attribute("id") && boneElement->Attribute("name"))
    {
      csString name(boneElement->Attribute("name"));
      csUInt8 id = atoi(boneElement->Attribute("id"));
      mesh->AddBoneName(name, id);
    }
  }

  //
  // currently we don't support global index buffers


  // 
  // lod the submeshes
  for (TiXmlElement *subMeshElement = subMeshesElement->FirstChildElement("subMesh");
       subMeshElement;
       subMeshElement = subMeshElement->NextSiblingElement("subMesh"))
  {
    csString name;
    csSize materialSlot = 0;
    csSize lod = 0;
    if (subMeshElement->Attribute("name"))
    {
      name = csString(subMeshElement->Attribute("name"));
    }
    if (subMeshElement->Attribute("materialSlot"))
    {
      materialSlot = atoi(subMeshElement->Attribute("materialSlot"));
    }
    if (subMeshElement->Attribute("lod"))
    {
      lod = atoi(subMeshElement->Attribute("lod"));
    }

    csResourceLocator subMeshLocator(csString(subMeshElement->GetText()));
    csSubMesh *subMesh = csResourceManager::Get()->GetOrLoad<csSubMesh>(subMeshLocator);
    if (subMesh)
    {
      mesh->AddMesh(subMesh, materialSlot, (csUInt8)lod, name);
      subMesh->Release();
    }
  }

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  return mesh;
}



