

#include <cobalt/loaders/csmeshassetxmlloader.hh>
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

csMeshAssetXMLLoader::csMeshAssetXMLLoader()
  : csBaseXMLLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csMeshAssetXMLLoader::~csMeshAssetXMLLoader()
{

}



bool csMeshAssetXMLLoader::CanLoad(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return std::string(element->Value()) == std::string("mesh");
}

const csClass *csMeshAssetXMLLoader::EvalClass(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  return csMesh::GetStaticClass();
}

iObject *csMeshAssetXMLLoader::Load(TiXmlElement *element, const csResourceLocator &locator, iObject *userData) const
{
  csMesh *mesh = new csMesh();

  TiXmlElement *materialSlotsElement = element->FirstChildElement("materialSlots");
  TiXmlElement *globalIndicesElement = element->FirstChildElement("globalIndices");
  TiXmlElement *subMeshesElement = element->FirstChildElement("subMeshes");

  if (!materialSlotsElement || !globalIndicesElement || !subMeshesElement)
  {
    return mesh;
  }


  // 
  // load the material slots
  std::vector<MaterialSlot> materialSlots;
  for (TiXmlElement *materialSlotElement = materialSlotsElement->FirstChildElement("materialSlot"); 
       materialSlotElement; 
       materialSlotElement = materialSlotElement->NextSiblingElement("materialSlot"))
  {
    if (!materialSlotElement->Attribute("name") || !materialSlotElement->Attribute("id"))
    {
      continue;
    }
    MaterialSlot slot;
    slot.name = std::string(materialSlotElement->Attribute("name"));
    slot.idx = atoi(materialSlotElement->Attribute("id"));
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
  for (TiXmlElement *subMeshElement = subMeshesElement->FirstChildElement("subMesh");
       subMeshElement;
       subMeshElement = subMeshElement->NextSiblingElement("subMesh"))
  {
    std::string name;
    csSize materialSlot = 0;
    csSize lod = 0;
    if (subMeshElement->Attribute("name"))
    {
      name = std::string(subMeshElement->Attribute("name"));
    }
    if (subMeshElement->Attribute("materialSlot"))
    {
      materialSlot = atoi(subMeshElement->Attribute("materialSlot"));
    }
    if (subMeshElement->Attribute("lod"))
    {
      lod = atoi(subMeshElement->Attribute("lod"));
    }

    csResourceLocator subMeshLocator(std::string(subMeshElement->GetText()));
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















