
#include <cobalt/loaders/csstaticmeshassetloader.hh>
#include <cobalt/entity/csgeometrydata.hh>
#include <cobalt/entity/csgeometrymesh.hh>
#include <cobalt/graphics/csmaterialdef.hh>
#include <cobalt/graphics/csmaterial.hh>
#include <cobalt/graphics/csmesh.hh>
#include <cobalt/graphics/csmultimaterial.hh>
#include <cobalt/graphics/csskinnedmesh.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/physics/csphysicsshape.hh>
#include <cobalt/csengine.hh>
#include <cobalt/csenums.hh>
#include <map>
#include <vector>



cs::StaticMeshAssetLoader::StaticMeshAssetLoader()
  : cs::iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::StaticMeshAssetLoader::~StaticMeshAssetLoader()
{

}

bool cs::StaticMeshAssetLoader::CanLoad(const std::string &typeID, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return typeID == std::string("MESH");// && name == std::string("DATA");
}

const cs::Class *cs::StaticMeshAssetLoader::EvalClass(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::Mesh::GetStaticClass();
}

cs::ResourceWrapper *cs::StaticMeshAssetLoader::Load(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  csUInt32 version, numMaterials, numSubMeshes, numIndices;
  inputStream >> version;

  if (version > CS_VERSION(1, 0, 0))
  {
    return nullptr;
  }

  cs::Mesh *mesh = new cs::Mesh();
  cs::MeshWrapper *meshWrapper = new cs::MeshWrapper(mesh);

  // 
  // read the material names of the mesh
  inputStream >> numMaterials;
  for (csUInt32 i = 0; i < numMaterials; ++i)
  {
    std::string name;
    inputStream >> name;
    mesh->AddMaterialName(name);
  }

  //
  // read and create the global index buffers that can be shared among multiple submeshes
  std::vector<cs::iIndexBuffer*> globalIndexBuffers;
  inputStream >> numIndices;
  for (csUInt32 i = 0; i < numIndices; ++i)
  {
    csSize indexBufferSize;
    inputStream >> indexBufferSize;
    unsigned char *buffer = new unsigned char[indexBufferSize];
    inputStream.Read(buffer, indexBufferSize);
    cs::iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(indexBufferSize, buffer, cs::eBDM_Static);
    globalIndexBuffers.push_back(indexBuffer);
    delete[] buffer;
  }

  // read the submeshes
  inputStream >> numSubMeshes;
  for (csUInt32 i = 0; i < numSubMeshes; ++i)
  {
    csSize materialIndex;
    csUInt8 lod;
    std::string name;

    inputStream
      >> materialIndex
      >> lod
      >> name;
    cs::SubMeshWrapper *subMesh = ReadSubMesh(inputStream, globalIndexBuffers, locator, userData);
    if (!subMesh)
    {
      CS_RELEASE(mesh);
      return nullptr;
    }
    mesh->AddMesh(subMesh, materialIndex, lod, name);
    CS_RELEASE(subMesh);
  }

  for (csSize i = 0, in = globalIndexBuffers.size(); i < in; ++i)
  {
    CS_RELEASE(globalIndexBuffers[i]);
  }
  globalIndexBuffers.clear();

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();
  const cs::BoundingBox& bbox = mesh->GetBoundingBox();
  bbox.Debug("StaticMeshLoader");

  return meshWrapper;
}


cs::SubMeshWrapper *cs::StaticMeshAssetLoader::ReadSubMesh(cs::AssetInputStream &inputStream, std::vector<cs::iIndexBuffer*> &globalIndexBuffers, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  cs::SubMesh *subMesh = new cs::SubMesh();
  cs::SubMeshWrapper *subMeshWrapper = new cs::SubMeshWrapper(subMesh);

  csUInt8 numVertexDeclaration;
  csUInt32 primType, indexType;
  inputStream >> primType >> indexType;

  subMesh->SetPrimitiveType((cs::ePrimitiveType)primType);
  subMesh->SetIndexType((cs::eDataType)indexType);
  //
  // read and create the vertex declaration
  std::vector<cs::VertexElement> vertexElements;
  inputStream >> numVertexDeclaration;
  for (csUInt32 i = 0; i < numVertexDeclaration; ++i)
  {
    csUInt32 streamType;
    csUInt32 dataType;
    csSize size;
    csUInt32 offset;
    csSize stride;
    csUInt8 stream;
    inputStream
      >> streamType
      >> dataType
      >> size
      >> offset
      >> stride
      >> stream;
    vertexElements.push_back(cs::VertexElement(
      (cs::eVertexStreamType)streamType,
      (cs::eDataType)dataType,
      size,
      offset,
      stride,
      stream));
  }
  vertexElements.push_back(cs::VertexElement());
  cs::iVertexDeclaration *vertexDeclaration = csEng->CreateVertexDeclaration(vertexElements.data());
  subMesh->SetVertexDeclaration(vertexDeclaration);
  CS_RELEASE(vertexDeclaration);

  //
  // read and create the vertex buffers;
  csUInt8 numVertexStreams;
  inputStream >> numVertexStreams;
  for (csUInt8 i = 0; i < numVertexStreams; ++i)
  {
    csSize vertexBufferSize;
    inputStream >> vertexBufferSize;
    unsigned char *buffer = new unsigned char[vertexBufferSize];
    inputStream.Read(buffer, vertexBufferSize);
    cs::iVertexBuffer *vertexBuffer = csEng->CreateVertexBuffer(vertexBufferSize, buffer, cs::eBDM_Static);
    subMesh->AddVertexBuffer(vertexBuffer);
    CS_RELEASE(vertexBuffer);
    delete[] buffer;
  }


  //
  // read and/or create the index buffer
  csInt8 sharedIndexBuffer;
  csSize offset, count;
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

    csSize indexBufferSize;
    inputStream >> indexBufferSize;
    unsigned char *buffer = new unsigned char[indexBufferSize];
    inputStream.Read(buffer, indexBufferSize);
    cs::iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(indexBufferSize, buffer, cs::eBDM_Static);
    delete[] buffer;
    subMesh->SetIndexBuffer(indexBuffer, count, offset);
  }

  //
  // read the bounding box
  cs::Vector3f min, max;
  inputStream >> min >> max;
  cs::BoundingBox bbox;
  bbox.Add(min);
  bbox.Add(max);
  bbox.Finish();
  subMesh->SetBoundingBox(bbox);

  return subMeshWrapper;
}
