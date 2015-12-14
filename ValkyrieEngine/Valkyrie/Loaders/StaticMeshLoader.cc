
#include <Valkyrie/Loaders/StaticMeshLoader.hh>
#include <Valkyrie/Graphics/Mesh.hh>
#include <Valkyrie/Graphics/IVertexDeclaration.hh>
#include <Valkyrie/Graphics/IGraphics.hh>
#include <Valkyrie/Engine.hh>
#include <map>
#include <vector>
#define VK_STATIC_MESH_LOADER_VERSION 1
#define VK_STATIC_MESH_LOADER_MAGIC_NUMBER 0x4853454D


enum HeaderEntry
{
  eHE_Type,
  eHE_Position,
  eHE_Size
};

enum EntryType
{
  eET_Geometry,
  eET_Mesh,
  eET_Collision,
  eET_Skeleton,
  eET_SkeletonAnimation
};

vkStaticMeshLoader::vkStaticMeshLoader()
  : IFileLoader()
{

}

vkStaticMeshLoader::~vkStaticMeshLoader()
{

}


bool vkStaticMeshLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return file->GetExtension() == vkString("staticmesh");
}


IObject *vkStaticMeshLoader::Load(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  // start at the beginning (should already be there)
  file->Seek(eSP_Set, 0L);

  vkUInt32 magicNumber;
  file->Read(&magicNumber, sizeof(vkUInt32));
  if (magicNumber != VK_STATIC_MESH_LOADER_MAGIC_NUMBER)
  {
    printf("The given file is no valid static mesh file: %s\n", locator.GetResourceFile().c_str());
    return 0;
  }

  vkUInt32 fileVersion;
  file->Read(&fileVersion, sizeof(vkUInt32));
  if (fileVersion > VK_STATIC_MESH_LOADER_VERSION)
  {
    printf("The given static mesh file has higher version as the load (%u > %u) : %s\n", fileVersion, VK_STATIC_MESH_LOADER_VERSION, locator.GetResourceFile().c_str());
    return 0;
  }

  vkUInt32 numberOfEntries;
  file->Read(&numberOfEntries, sizeof(vkUInt32));
  for (vkUInt32 i = 0; i < numberOfEntries; ++i)
  {
    vkString entryName = ReadString(file);
    if (locator.GetResourceName().length() == 0 || locator.GetResourceName() == entryName)
    {
      vkUInt32 header[3];
      file->Read(header, sizeof(vkUInt32) * 3);

      EntryType type = (EntryType)header[eHE_Type];
      vkUInt32 pos = header[eHE_Position];

      file->Seek(eSP_Set, (long)pos);
      switch (type)
      {
      case eET_Geometry:
        printf("Cannot reader GeometryData. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
        return 0;

      case eET_Mesh:
        return ReadMesh(fileVersion, file, locator, userData);

      case eET_Collision:
        printf("Cannot reader Collision. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
        return 0;
      case eET_Skeleton:
        printf("Cannot reader Skeleton. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
        return 0;
      case eET_SkeletonAnimation:
        printf("Cannot reader SkeletonAnimation. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
        return 0;
      }

    }
  }
  return 0;
}


vkMesh *vkStaticMeshLoader::ReadMesh(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  std::map<vkString, vkUInt32> materialIDs;
  std::map<vkUInt32, vkString> materialNames;

  vkUInt32 numMaterials;
  file->Read(&numMaterials, sizeof(vkUInt32));

  printf("Materials:");
  for (vkUInt32 i = 0; i < numMaterials; ++i)
  {
    vkString materialName = ReadString(file);
    materialIDs[materialName] = i;
    materialNames[i] = materialName;
    printf("  '%s' @ %u\n", materialName.c_str(), i);
  }


  vkUInt32 numSubMeshes;
  file->Read(&numSubMeshes, sizeof(vkUInt32));

  for (vkUInt32 i = 0; i < numSubMeshes; ++i)
  {
    vkSubMesh *subMesh = ReadSubMesh(fileVersion, file, locator, userData);



  }

  return 0;
}


vkSubMesh *vkStaticMeshLoader::ReadSubMesh(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  vkSubMesh *subMesh = new vkSubMesh();

  vkUInt32 materialIndex, LOD;
  file->Read(&materialIndex, sizeof(vkUInt32));
  file->Read(&LOD, sizeof(vkUInt32));

  IVertexDeclaration *decl = ReadVertexDeclaration(file);

  if (!decl)
  {
    subMesh->Release();
    return 0;
  }

  subMesh->SetVertexDeclaration(decl);



  vkUInt32 numVertices, numStreams;
  file->Read(&numVertices, sizeof(vkUInt32));
  file->Read(&numStreams, sizeof(vkUInt32));

  for (vkUInt32 i = 0; i < numStreams; ++i)
  {
    vkUInt32 stride;
    file->Read(&stride, sizeof(vkUInt32));
    unsigned char *buffer = new unsigned char[numVertices * stride];
    file->Read(buffer, numVertices * stride);

    IVertexBuffer *vb = vkEngine::Get()->GetRenderer()->CreateVertexBuffer(numVertices * stride, buffer, eBDM_Static);
    subMesh->AddVertexBuffer(vb);
  }


  vkUInt32 numIndices, indexType;
  file->Read(&numIndices, sizeof(vkUInt32));
}


IVertexDeclaration *vkStaticMeshLoader::ReadVertexDeclaration(IFile *file) const
{
  vkUInt32 numVertexDeclarations;
  file->Read(&numVertexDeclarations, sizeof(vkUInt32));

  std::vector<vkVertexElement> elements;
  for (vkUInt32 i = 0; i < numVertexDeclarations; ++i)
  {
    vkUInt32 data[6];
    file->Read(data, sizeof(vkUInt32) * 6);

    elements.push_back(vkVertexElement(
      (vkVertexStreamType)data[0],
      (vkDataType)data[1],
      data[2],
      data[3],
      data[4],
      data[5]
      ));
  }
  elements.push_back(vkVertexElement());

  IVertexDeclaration *decl = vkEngine::Get()->GetRenderer()->CreateVertexDeclaration(elements.data());

  return decl;
}


vkString vkStaticMeshLoader::ReadString(IFile *file) const
{
  vkUInt32 length;
  file->Read(&length, sizeof(vkUInt32));

  if (length == 0)
  {
    return vkString();
  }

  char *buffer = new char[length];
  file->Read(buffer, length);
  vkString res(buffer);
  delete[] buffer;
  return res;
}