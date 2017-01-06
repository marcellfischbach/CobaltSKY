

#include <Valkyrie/loaders/vkskinnedmeshassetxmlloader.hh>
#include <Valkyrie/graphics/vkskinnedmesh.hh>
#include <Valkyrie/graphics/vksubmesh.hh>
#include <Valkyrie/graphics/igraphics.hh>
#include <Valkyrie/graphics/ivertexbuffer.hh>
#include <Valkyrie/graphics/ivertexdeclaration.hh>
#include <Valkyrie/graphics/iindexbuffer.hh>

#include <Valkyrie/vkengine.hh>
#include <algorithm>


struct MaterialSlot
{
  vkString name;
  vkSize idx;
};

namespace
{
bool slot_sort(MaterialSlot &slot0, MaterialSlot &slot1)
{
  return slot0.idx < slot1.idx;
}
}

vkSkinnedMeshAssetXMLLoader::vkSkinnedMeshAssetXMLLoader()
  : vkBaseXMLLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkSkinnedMeshAssetXMLLoader::~vkSkinnedMeshAssetXMLLoader()
{

}



bool vkSkinnedMeshAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("skinnedMesh");
}

const vkClass *vkSkinnedMeshAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkSkinnedMesh::GetStaticClass();
}

IObject *vkSkinnedMeshAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  vkSkinnedMesh *mesh = new vkSkinnedMesh();

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
    slot.name = vkString(materialSlotElement->Attribute("name"));
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
      vkString name(boneElement->Attribute("name"));
      vkUInt8 id = atoi(boneElement->Attribute("id"));
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
    vkString name;
    vkSize materialSlot = 0;
    vkSize lod = 0;
    if (subMeshElement->Attribute("name"))
    {
      name = vkString(subMeshElement->Attribute("name"));
    }
    if (subMeshElement->Attribute("materialSlot"))
    {
      materialSlot = atoi(subMeshElement->Attribute("materialSlot"));
    }
    if (subMeshElement->Attribute("lod"))
    {
      lod = atoi(subMeshElement->Attribute("lod"));
    }

    vkResourceLocator subMeshLocator(vkString(subMeshElement->GetText()));
    vkSubMesh *subMesh = vkResourceManager::Get()->GetOrLoad<vkSubMesh>(subMeshLocator);
    if (subMesh)
    {
      mesh->AddMesh(subMesh, materialSlot, (vkUInt8)lod, name);
      subMesh->Release();
    }
  }

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  return mesh;
}



