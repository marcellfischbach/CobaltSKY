
#include <Valkyrie/Loaders/vkstaticmeshassetloader.hh>
#include <Valkyrie/entity/vkgeometrydata.hh>
#include <Valkyrie/entity/vkgeometrymesh.hh>
#include <Valkyrie/graphics/vkmaterial.hh>
#include <Valkyrie/graphics/vkmaterialinstance.hh>
#include <Valkyrie/graphics/vkmesh.hh>
#include <Valkyrie/graphics/vkmultimaterial.hh>
#include <Valkyrie/graphics/vkskinnedmesh.hh>
#include <Valkyrie/graphics/vksubmesh.hh>
#include <Valkyrie/graphics/iindexbuffer.hh>
#include <Valkyrie/graphics/ivertexbuffer.hh>
#include <Valkyrie/graphics/ivertexdeclaration.hh>
#include <Valkyrie/graphics/igraphics.hh>
#include <Valkyrie/physics/vkphysicsshapecontainer.hh>
#include <Valkyrie/vkengine.hh>
#include <Valkyrie/vkenums.hh>
#include <map>
#include <vector>



vkStaticMeshAssetLoader::vkStaticMeshAssetLoader()
  : IAssetLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkStaticMeshAssetLoader::~vkStaticMeshAssetLoader()
{

}

bool vkStaticMeshAssetLoader::CanLoad(const vkString &typeID, const vkResourceLocator &locator, IObject *userData) const
{
  return typeID == vkString("MESH");// && name == vkString("DATA");
}

const vkClass *vkStaticMeshAssetLoader::EvalClass(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  return vkMesh::GetStaticClass();
}

IObject *vkStaticMeshAssetLoader::Load(vkAssetInputStream &inputStream, const vkResourceLocator &locator, IObject *userData) const
{
  vkUInt32 version, numMaterials, numSubMeshes, numIndices;
  inputStream >> version;

  if (version > VK_VERSION(1, 0, 0))
  {
    return 0;
  }

  vkMesh *mesh = new vkMesh();


  // 
  // read the material names of the mesh
  inputStream >> numMaterials;
  for (vkUInt32 i = 0; i < numMaterials; ++i)
  {
    vkString name;
    inputStream >> name;
    mesh->AddMaterialName(name);
  }

  //
  // read and create the global index buffers that can be shared among multiple submeshes
  std::vector<IIndexBuffer*> globalIndexBuffers;
  inputStream >> numIndices;
  for (vkUInt32 i = 0; i < numIndices; ++i)
  {
    vkSize indexBufferSize;
    inputStream >> indexBufferSize;
    unsigned char *buffer = new unsigned char[indexBufferSize];
    inputStream.Read(buffer, indexBufferSize);
    IIndexBuffer *indexBuffer = vkEng->CreateIndexBuffer(indexBufferSize, buffer, eBDM_Static);
    globalIndexBuffers.push_back(indexBuffer);
    delete[] buffer;
  }

  // read the submeshes
  inputStream >> numSubMeshes;
  for (vkUInt32 i = 0; i < numSubMeshes; ++i)
  {
    vkSize materialIndex;
    vkUInt8 lod;
    vkString name;

    inputStream
      >> materialIndex
      >> lod
      >> name;
    vkSubMesh *subMesh = ReadSubMesh(inputStream, globalIndexBuffers, locator, userData);
    if (!subMesh)
    {
      VK_RELEASE(mesh);
      return 0;
    }
    mesh->AddMesh(subMesh, materialIndex, lod, name);
    VK_RELEASE(subMesh);
  }

  for (vkSize i = 0, in = globalIndexBuffers.size(); i < in; ++i)
  {
    VK_RELEASE(globalIndexBuffers[i]);
  }
  globalIndexBuffers.clear();

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  const vkBoundingBox& bbox = mesh->GetBoundingBox();
  bbox.Debug("StaticMeshLoader");

  return mesh;
}


vkSubMesh *vkStaticMeshAssetLoader::ReadSubMesh(vkAssetInputStream &inputStream, std::vector<IIndexBuffer*> &globalIndexBuffers, const vkResourceLocator &locator, IObject *userData) const
{
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
    subMesh->SetIndexBuffer(globalIndexBuffers[sharedIndexBuffer], count, offset);
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
