

#include <valkyrie/loaders/vkmeshassetxmlloader.hh>
#include <valkyrie/graphics/vkmesh.hh>
#include <valkyrie/graphics/vksubmesh.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <valkyrie/graphics/iindexbuffer.hh>

#include <valkyrie/vkengine.hh>
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

vkMeshAssetXMLLoader::vkMeshAssetXMLLoader()
  : vkBaseXMLLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkMeshAssetXMLLoader::~vkMeshAssetXMLLoader()
{

}



bool vkMeshAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkString(element->Value()) == vkString("mesh");
}

const vkClass *vkMeshAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  return vkMesh::GetStaticClass();
}

iObject *vkMeshAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, iObject *userData) const
{
  vkMesh *mesh = new vkMesh();

  TiXmlElement *materialSlotsElement = element->FirstChildElement("materialSlots");
  TiXmlElement *globalIndicesElement = element->FirstChildElement("globalIndices");
  TiXmlElement *subMeshesElement = element->FirstChildElement("subMeshes");

  if (!materialSlotsElement || !globalIndicesElement || !subMeshesElement)
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















