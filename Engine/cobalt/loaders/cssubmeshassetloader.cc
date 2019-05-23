

#include <cobalt/loaders/cssubmeshassetloader.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/iindexbuffer.hh>

#include <cobalt/csengine.hh>
#include <algorithm>

cs::SubMeshAssetLoader::SubMeshAssetLoader()
  : cs::iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

cs::SubMeshAssetLoader::~SubMeshAssetLoader()
{

}

bool cs::SubMeshAssetLoader::CanLoad(const std::string &typeID, const cs::ResourceLocator &locator, cs::iObject *userData)const
{
  return typeID == std::string("SUBMESH");
}

const cs::Class *cs::SubMeshAssetLoader::EvalClass(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  return cs::SubMesh::GetStaticClass();
}

cs::ResourceWrapper *cs::SubMeshAssetLoader::Load(cs::AssetInputStream &inputStream, const cs::ResourceLocator &locator, cs::iObject *userData) const
{
  csUInt32 version;

  inputStream >> version;
  if (version > CS_VERSION(1, 0, 0))
  {
    return nullptr;
  }
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
    if (!userData)
    {
      subMesh->Release();
      return nullptr;
    }
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
