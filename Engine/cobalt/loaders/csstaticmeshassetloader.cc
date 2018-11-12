
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



csStaticMeshAssetLoader::csStaticMeshAssetLoader()
  : iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csStaticMeshAssetLoader::~csStaticMeshAssetLoader()
{

}

bool csStaticMeshAssetLoader::CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData) const
{
  return typeID == std::string("MESH");// && name == std::string("DATA");
}

const csClass *csStaticMeshAssetLoader::EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  return csMesh::GetStaticClass();
}

csResourceWrapper *csStaticMeshAssetLoader::Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  csUInt32 version, numMaterials, numSubMeshes, numIndices;
  inputStream >> version;

  if (version > CS_VERSION(1, 0, 0))
  {
    return nullptr;
  }

  csMesh *mesh = new csMesh();
  csMeshWrapper *meshWrapper = new csMeshWrapper(mesh);

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
  std::vector<iIndexBuffer*> globalIndexBuffers;
  inputStream >> numIndices;
  for (csUInt32 i = 0; i < numIndices; ++i)
  {
    csSize indexBufferSize;
    inputStream >> indexBufferSize;
    unsigned char *buffer = new unsigned char[indexBufferSize];
    inputStream.Read(buffer, indexBufferSize);
    iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(indexBufferSize, buffer, eBDM_Static);
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
    csSubMeshWrapper *subMesh = ReadSubMesh(inputStream, globalIndexBuffers, locator, userData);
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
  const csBoundingBox& bbox = mesh->GetBoundingBox();
  bbox.Debug("StaticMeshLoader");

  return meshWrapper;
}


csSubMeshWrapper *csStaticMeshAssetLoader::ReadSubMesh(csAssetInputStream &inputStream, std::vector<iIndexBuffer*> &globalIndexBuffers, const csResourceLocator &locator, iObject *userData) const
{
  csSubMesh *subMesh = new csSubMesh();
  csSubMeshWrapper *subMeshWrapper = new csSubMeshWrapper(subMesh);

  csUInt8 numVertexDeclaration;
  csUInt32 primType, indexType;
  inputStream >> primType >> indexType;

  subMesh->SetPrimitiveType((csPrimitiveType)primType);
  subMesh->SetIndexType((csDataType)indexType);
  //
  // read and create the vertex declaration
  std::vector<csVertexElement> vertexElements;
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
    vertexElements.push_back(csVertexElement(
      (csVertexStreamType)streamType,
      (csDataType)dataType,
      size,
      offset,
      stride,
      stream));
  }
  vertexElements.push_back(csVertexElement());
  iVertexDeclaration *vertexDeclaration = csEng->CreateVertexDeclaration(vertexElements.data());
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
    iVertexBuffer *vertexBuffer = csEng->CreateVertexBuffer(vertexBufferSize, buffer, eBDM_Static);
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
    iIndexBuffer *indexBuffer = csEng->CreateIndexBuffer(indexBufferSize, buffer, eBDM_Static);
    delete[] buffer;
    subMesh->SetIndexBuffer(indexBuffer, count, offset);
  }

  //
  // read the bounding box
  csVector3f min, max;
  inputStream >> min >> max;
  csBoundingBox bbox;
  bbox.Add(min);
  bbox.Add(max);
  bbox.Finish();
  subMesh->SetBoundingBox(bbox);

  return subMeshWrapper;
}
