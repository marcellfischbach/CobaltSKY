

#include <valkyrie/loaders/vksubmeshassetxmlloader.hh>
#include <valkyrie/graphics/vksubmesh.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <valkyrie/graphics/ivertexbuffer.hh>
#include <valkyrie/graphics/ivertexdeclaration.hh>
#include <valkyrie/graphics/iindexbuffer.hh>

#include <valkyrie/vkengine.hh>
#include <algorithm>

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
