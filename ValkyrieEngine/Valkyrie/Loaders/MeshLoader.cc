

#include <Valkyrie/Loaders/MeshLoader.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>

#include <Valkyrie/Engine.hh>
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



bool vkMeshAssetXMLLoader::CanLoad(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkString(element->Value()) == vkString("mesh");
}

const vkClass *vkMeshAssetXMLLoader::EvalClass(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
{
  return vkMesh::GetStaticClass();
}

IObject *vkMeshAssetXMLLoader::Load(TiXmlElement *element, const vkResourceLocator &locator, IObject *userData) const
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











vkSubMeshAssetLoader::vkSubMeshAssetLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkSubMeshAssetLoader::~vkSubMeshAssetLoader()
{

}

bool vkSubMeshAssetLoader::CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData)const
{
  return typeID == vkString("SUBMESH");
}

const vkClass *vkSubMeshAssetLoader::EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  return vkSubMesh::GetStaticClass();
}

IObject *vkSubMeshAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  vkUInt32 version;

  inputStream >> version;
  if (version > VK_VERSION(1, 0, 0))
  {
    return 0;
  }
  vkSubMesh *subMesh = new vkSubMesh();

  vkUInt8 numVertexDeclaration;
  vkUInt32 primType, indexType;
  inputStream >> primType >> indexType;

  subMesh->SetPrimitiveType((vkPrimitiveType)primType);
  subMesh->SetIndexType((vkDataType)indexType);
  //
  // read and create the vertex declaration
  std::vector<vkVertexElement> vertexElements;
  inputStream >> numVertexDeclaration;
  for (vkUInt32 i = 0; i < numVertexDeclaration; ++i)
  {
    vkUInt32 streamType;
    vkUInt32 dataType;
    vkSize size;
    vkUInt32 offset;
    vkSize stride;
    vkUInt8 stream;
    inputStream
      >> streamType
      >> dataType
      >> size
      >> offset
      >> stride
      >> stream;
    vertexElements.push_back(vkVertexElement(
      (vkVertexStreamType)streamType,
      (vkDataType)dataType,
      size,
      offset,
      stride,
      stream));
  }
  vertexElements.push_back(vkVertexElement());
  IVertexDeclaration *vertexDeclaration = vkEng->CreateVertexDeclaration(vertexElements.data());
  subMesh->SetVertexDeclaration(vertexDeclaration);
  VK_RELEASE(vertexDeclaration);

  //
  // read and create the vertex buffers;
  vkUInt8 numVertexStreams;
  inputStream >> numVertexStreams;
  for (vkUInt8 i = 0; i < numVertexStreams; ++i)
  {
    vkSize vertexBufferSize;
    inputStream >> vertexBufferSize;
    unsigned char *buffer = new unsigned char[vertexBufferSize];
    inputStream.Read(buffer, vertexBufferSize);
    IVertexBuffer *vertexBuffer = vkEng->CreateVertexBuffer(vertexBufferSize, buffer, eBDM_Static);
    subMesh->AddVertexBuffer(vertexBuffer);
    VK_RELEASE(vertexBuffer);
    delete[] buffer;
  }


  //
  // read and/or create the index buffer
  vkInt8 sharedIndexBuffer;
  vkSize offset, count;
  inputStream
    >> sharedIndexBuffer
    >> count
    >> offset;
  if (sharedIndexBuffer != -1)
  {
    if (!userData)
    {
      subMesh->Release();
      return 0;
    }
  }
  else
  {

    vkSize indexBufferSize;
    inputStream >> indexBufferSize;
    unsigned char *buffer = new unsigned char[indexBufferSize];
    inputStream.Read(buffer, indexBufferSize);
    IIndexBuffer *indexBuffer = vkEng->CreateIndexBuffer(indexBufferSize, buffer, eBDM_Static);
    delete[] buffer;
    subMesh->SetIndexBuffer(indexBuffer, count, offset);
  }

  //
  // read the bounding box
  vkVector3f min, max;
  inputStream >> min >> max;
  vkBoundingBox bbox;
  bbox.Add(min);
  bbox.Add(max);
  bbox.Finish();
  subMesh->SetBoundingBox(bbox);

  return subMesh;
}
