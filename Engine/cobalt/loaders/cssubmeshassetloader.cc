

#include <cobalt/loaders/cssubmeshassetloader.hh>
#include <cobalt/graphics/cssubmesh.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <cobalt/graphics/ivertexdeclaration.hh>
#include <cobalt/graphics/iindexbuffer.hh>

#include <cobalt/csengine.hh>
#include <algorithm>

csSubMeshAssetLoader::csSubMeshAssetLoader()
  : iAssetLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csSubMeshAssetLoader::~csSubMeshAssetLoader()
{

}

bool csSubMeshAssetLoader::CanLoad(const std::string &typeID, const csResourceLocator &locator, iObject *userData)const
{
  return typeID == std::string("SUBMESH");
}

const csClass *csSubMeshAssetLoader::EvalClass(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  return csSubMesh::GetStaticClass();
}

csResourceWrapper *csSubMeshAssetLoader::Load(csAssetInputStream &inputStream, const csResourceLocator &locator, iObject *userData) const
{
  csUInt32 version;

  inputStream >> version;
  if (version > CS_VERSION(1, 0, 0))
  {
    return nullptr;
  }
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
