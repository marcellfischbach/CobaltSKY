
#include <Valkyrie/loaders/vkstaticmeshloader.hh>
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

enum GeometryType
{
  eGT_GeometryMesh,
  eGT_GeometryCollection,
  eGT_GeometryLOD
};

vkStaticMeshLoader::vkStaticMeshLoader()
  : IFileLoader()
{
  VK_CLASS_GEN_CONSTR;
}

vkStaticMeshLoader::~vkStaticMeshLoader()
{

}


bool vkStaticMeshLoader::CanLoad(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return file->GetExtension() == vkString("staticmesh");
}


const vkClass *vkStaticMeshLoader::EvalClass(IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  return vkMesh::GetStaticClass();
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

  std::map<vkString, HeaderEntry> entries;

  vkString entryToLoad;

  vkUInt32 numberOfEntries;
  file->Read(&numberOfEntries, sizeof(vkUInt32));
  for (vkUInt32 i = 0; i < numberOfEntries; ++i)
  {
    HeaderEntry entry;
    entry.name = ReadString(file);
    entry.obj = 0;
    file->Read(&entry.type, sizeof(vkUInt32));
    file->Read(&entry.position, sizeof(vkUInt32));
    file->Read(&entry.size, sizeof(vkUInt32));


    entries[entry.name] = entry;

    if (entryToLoad.length() == 0)
    {
      entryToLoad = entry.name;
    }
  }


  if (locator.GetResourceName().length() != 0)
  {
    entryToLoad = locator.GetResourceName();
  }

  return ReadEntry(entries, entryToLoad, fileVersion, file, locator, userData);

}

IObject *vkStaticMeshLoader::ReadEntry(std::map<vkString, HeaderEntry> &entries, const vkString &entryName, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  std::map<vkString, HeaderEntry>::iterator it = entries.find(entryName);
  if (it == entries.end())
  {
    return 0;
  }

  HeaderEntry &he = it->second;
  if (he.obj)
  {
    // who ever gets the object back is the owner of the object
    he.obj->AddRef();
    return he.obj;
  }

  vkUInt32 pos = he.position;

  file->Seek(eSP_Set, (long)pos);
  switch (he.type)
  {
  case eET_Geometry:
    return ReadGeometry(entries, fileVersion, file, locator, userData);

  case eET_Mesh:
    return ReadMesh(fileVersion, file, locator, userData);

  case eET_Collision:
    return ReadCollision(fileVersion, file, locator, userData);

  case eET_Skeleton:
    printf("Cannot reader Skeleton. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    return 0;
  case eET_SkeletonAnimation:
    printf("Cannot reader SkeletonAnimation. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    return 0;
  }

  return 0;
}


vkMesh *vkStaticMeshLoader::ReadMesh(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  std::map<vkString, vkUInt32> materialIDs;
  std::map<vkUInt32, vkString> materialNames;

  vkUInt32 numMaterials;
  file->Read(&numMaterials, sizeof(vkUInt32));

  vkMesh *mesh = new vkMesh();
  for (vkUInt32 i = 0; i < numMaterials; ++i)
  {
    vkString materialName = ReadString(file);
    materialIDs[materialName] = i;
    materialNames[i] = materialName;
    mesh->AddMaterialName(materialName);
  }

  vkUInt32 numSubMeshes;
  file->Read(&numSubMeshes, sizeof(vkUInt32));

  for (vkUInt32 i = 0; i < numSubMeshes; ++i)
  {
    if (!ReadSubMesh(mesh, fileVersion, file, locator, userData))
    {
      mesh->Release();
      return 0;
    }
  }

  mesh->OptimizeDataStruct();
  mesh->UpdateBoundingBox();

  return mesh;
}


bool vkStaticMeshLoader::ReadSubMesh(vkMesh *mesh, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  vkSubMesh *subMesh = new vkSubMesh();

  vkUInt32 materialIndex, LOD;
  file->Read(&materialIndex, sizeof(vkUInt32));
  file->Read(&LOD, sizeof(vkUInt32));

  IVertexDeclaration *decl = ReadVertexDeclaration(file);

  if (!decl)
  {
    subMesh->Release();
    return false;
  }

  subMesh->SetVertexDeclaration(decl);
  decl->Release();



  vkUInt32 numVertices, numStreams;
  file->Read(&numVertices, sizeof(vkUInt32));
  file->Read(&numStreams, sizeof(vkUInt32));

  for (vkUInt32 i = 0; i < numStreams; ++i)
  {
    vkUInt32 stride;
    file->Read(&stride, sizeof(vkUInt32));
    unsigned char *buffer = new unsigned char[numVertices * stride];
    file->Read(buffer, numVertices * stride);

    IVertexBuffer *vb = vkEng->CreateVertexBuffer(numVertices * stride, buffer, eBDM_Static);
    delete[] buffer;
    if (!vb)
    {
      printf ("Vertex stream could not be generated: %d %s:%s\n", i, locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
      subMesh->Release();
      return false;
    }
    subMesh->AddVertexBuffer(vb);
    vb->Release();
  }


  vkUInt32 numIndices, indexType;
  file->Read(&numIndices, sizeof(vkUInt32));
  file->Read(&indexType, sizeof(vkUInt32));

  vkUInt32 idxSize = 0;
  switch (indexType)
  {
  case eDT_UnsignedShort:
    idxSize = sizeof(vkUInt16);
    break;
  case eDT_UnsignedInt:
    idxSize = sizeof(vkUInt32);
    break;
  default:
    printf("Invalid index data type: %d %s:%s\n", indexType, locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    subMesh->Release();
    return false;
  }
  unsigned char *buffer = new unsigned char[numIndices * idxSize];
  file->Read(buffer, numIndices * idxSize);
  IIndexBuffer *ib = vkEng->CreateIndexBuffer(numIndices * idxSize, buffer, eBDM_Static);
  delete[] buffer;
  if (!ib)
  {
    subMesh->Release();
    return false;
  }

  subMesh->SetIndexBuffer(ib, numIndices, 0);

  vkVector3f bboxMin;
  vkVector3f bboxMax;
  file->Read(&bboxMin, sizeof(float) * 3);
  file->Read(&bboxMax, sizeof(float) * 3);
  vkBoundingBox bbox;
  bbox.Add(bboxMin);
  bbox.Add(bboxMax);
  bbox.Finish();
  subMesh->SetBoundingBox(bbox);

  mesh->AddMesh(subMesh, materialIndex, LOD);

  return true;
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

  IVertexDeclaration *decl = vkEng->CreateVertexDeclaration(elements.data());

  return decl;
}


vkPhysicsShapeContainer *vkStaticMeshLoader::ReadCollision(vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  vkPhysicsShapeContainer *container = new vkPhysicsShapeContainer();
  vkUInt32 numShapes;
  file->Read(&numShapes, sizeof(vkUInt32));
  for (vkUInt32 i = 0; i < numShapes; ++i)
  {
    vkUInt32 shapeTypeI;
    file->Read(&shapeTypeI, sizeof(vkUInt32));
    vkMatrix4f trans;
    file->Read(&trans, sizeof(vkMatrix4f));
    vkPhysGeometryType type = static_cast<vkPhysGeometryType>(shapeTypeI);
    switch (type)
    {
    case ePGT_Box:
      {
        vkPhysGeometry geom;
        memset(&geom, 0, sizeof(geom));
        geom.Type = type;
        file->Read(&geom.Dimensions, sizeof(vkVector3f));
        IPhysicsShape *shape = vkEng->CreateShape(geom);
        shape->SetLocalTransform(trans);
        container->AddShape(shape);
      }
      break;
    default:
      printf("Reading physics of type %d not implemented yet.\n", type);
      break;
    }
  }

  return container;
}


vkGeometryData* vkStaticMeshLoader::ReadGeometry(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  vkUInt32 type;
  file->Read(&type, sizeof(vkUInt32));

  switch (type)
  {
  case eGT_GeometryMesh:
    return ReadGeometryMesh(entries, fileVersion, file, locator, userData);

  case eGT_GeometryCollection:
    printf("Cannot reader geometry collection. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    return 0;

  case eGT_GeometryLOD:
    printf("Cannot reader geometry lod. Not implemented yet: %s:%s\n", locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    return 0;

  }

  return 0;
}


vkGeometryMesh* vkStaticMeshLoader::ReadGeometryMesh(std::map<vkString, HeaderEntry> &entries, vkUInt32 fileVersion, IFile *file, const vkResourceLocator &locator, IObject *userData) const
{
  enum ReadMode
  {
    eRM_Internal,
    eRM_External
  };
  vkMatrix4f localTransformation;
  file->Read(&localTransformation, sizeof(float) * 16);

  vkUInt32 readMode;
  file->Read(&readMode, sizeof(vkUInt32));

  IObject *meshObj = 0;
  switch (readMode)
  {
  case eRM_Internal:
    {
      vkString name = ReadString(file);
      vkSize currentPosition = file->Tell();
      meshObj = ReadEntry(entries, name, fileVersion, file, locator, userData);
      file->Seek(eSP_Set, (long)currentPosition);
    }
    break;
  case eRM_External:
    {
      vkString resourceFile = ReadString(file);
      vkString resourceName = ReadString(file);
      meshObj = vkResourceManager::Get()->GetOrLoad(vkResourceLocator(resourceFile, resourceName), userData);
      if (meshObj)
      {
        // make me the owner, otherwise there is a difference between GetOrLoad and ReadEntry
        meshObj->AddRef();
      }
    }
    break;
  }

  if (!meshObj)
  {
    return 0;
  }

  vkMesh *mesh = vkQueryClass<vkMesh>(meshObj);
  if (!mesh)
  {
    meshObj->Release();
    return 0;
  }

  vkMultiMaterial *material = ReadMultiMaterial(file);


  vkGeometryMesh *geometryMesh = new vkGeometryMesh();
  geometryMesh->SetMesh(mesh);
  geometryMesh->SetMaterial(material);

  return geometryMesh;
}

vkMultiMaterial *vkStaticMeshLoader::ReadMultiMaterial(IFile *file) const
{
  vkUInt32 numberOfMaterials;
  file->Read(&numberOfMaterials, sizeof(vkUInt32));

  vkResourceManager *mgr = vkResourceManager::Get();
  vkMultiMaterial *multiMaterial = new vkMultiMaterial();
  for (vkUInt32 i = 0; i < numberOfMaterials; ++i)
  {
    vkString name = ReadString(file);
    vkMaterialInstance *inst = mgr->GetOrLoad<vkMaterialInstance>(vkResourceLocator("${materials}/materials.xml", name));
    if (inst)
    {
      multiMaterial->AddMaterialInstance(inst);
    }
  }
  return multiMaterial;
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





