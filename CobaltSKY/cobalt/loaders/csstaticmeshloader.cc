
#include <cobalt/loaders/csstaticmeshloader.hh>
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
#include <cobalt/physics/csphysicsshapecontainer.hh>
#include <cobalt/csengine.hh>
#include <cobalt/csenums.hh>
#include <map>
#include <vector>

#define CS_STATIC_MESH_LOADER_VERSION 1
#define CS_STATIC_MESH_LOADER_MAGIC_NUMBER 0x4853454D


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

csStaticMeshLoader::csStaticMeshLoader()
  : iFileLoader()
{
  CS_CLASS_GEN_CONSTR;
}

csStaticMeshLoader::~csStaticMeshLoader()
{

}


bool csStaticMeshLoader::CanLoad(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  return file->GetExtension() == std::string("staticmesh");
}


const csClass *csStaticMeshLoader::EvalClass(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  return csMesh::GetStaticClass();
}


iObject *csStaticMeshLoader::Load(iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  // start at the beginning (should already be there)
  file->Seek(eSP_Set, 0L);

  csUInt32 magicNumber;
  file->Read(&magicNumber, sizeof(csUInt32));
  if (magicNumber != CS_STATIC_MESH_LOADER_MAGIC_NUMBER)
  {
    printf("The given file is no valid static mesh file: %s\n", locator.GetResourceFile().c_str());
    return 0;
  }

  csUInt32 fileVersion;
  file->Read(&fileVersion, sizeof(csUInt32));
  if (fileVersion > CS_STATIC_MESH_LOADER_VERSION)
  {
    printf("The given static mesh file has higher version as the load (%u > %u) : %s\n", fileVersion, CS_STATIC_MESH_LOADER_VERSION, locator.GetResourceFile().c_str());
    return 0;
  }

  std::map<std::string, HeaderEntry> entries;

  std::string entryToLoad;

  csUInt32 numberOfEntries;
  file->Read(&numberOfEntries, sizeof(csUInt32));
  for (csUInt32 i = 0; i < numberOfEntries; ++i)
  {
    HeaderEntry entry;
    entry.name = ReadString(file);
    entry.obj = 0;
    file->Read(&entry.type, sizeof(csUInt32));
    file->Read(&entry.position, sizeof(csUInt32));
    file->Read(&entry.size, sizeof(csUInt32));


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

iObject *csStaticMeshLoader::ReadEntry(std::map<std::string, HeaderEntry> &entries, const std::string &entryName, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  std::map<std::string, HeaderEntry>::iterator it = entries.find(entryName);
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

  csUInt32 pos = he.position;

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


csMesh *csStaticMeshLoader::ReadMesh(csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  std::map<std::string, csUInt32> materialIDs;
  std::map<csUInt32, std::string> materialNames;

  csUInt32 numMaterials;
  file->Read(&numMaterials, sizeof(csUInt32));

  csMesh *mesh = new csMesh();
  for (csUInt32 i = 0; i < numMaterials; ++i)
  {
    std::string materialName = ReadString(file);
    materialIDs[materialName] = i;
    materialNames[i] = materialName;
    mesh->AddMaterialName(materialName);
  }

  csUInt32 numSubMeshes;
  file->Read(&numSubMeshes, sizeof(csUInt32));

  for (csUInt32 i = 0; i < numSubMeshes; ++i)
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


bool csStaticMeshLoader::ReadSubMesh(csMesh *mesh, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  csSubMesh *subMesh = new csSubMesh();

  csUInt32 materialIndex, LOD;
  file->Read(&materialIndex, sizeof(csUInt32));
  file->Read(&LOD, sizeof(csUInt32));

  iVertexDeclaration *decl = ReadVertexDeclaration(file);

  if (!decl)
  {
    subMesh->Release();
    return false;
  }

  subMesh->SetVertexDeclaration(decl);
  decl->Release();



  csUInt32 numVertices, numStreams;
  file->Read(&numVertices, sizeof(csUInt32));
  file->Read(&numStreams, sizeof(csUInt32));

  for (csUInt32 i = 0; i < numStreams; ++i)
  {
    csUInt32 stride;
    file->Read(&stride, sizeof(csUInt32));
    unsigned char *buffer = new unsigned char[numVertices * stride];
    file->Read(buffer, numVertices * stride);

    iVertexBuffer *vb = csEng->CreateVertexBuffer(numVertices * stride, buffer, eBDM_Static);
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


  csUInt32 numIndices, indexType;
  file->Read(&numIndices, sizeof(csUInt32));
  file->Read(&indexType, sizeof(csUInt32));

  csUInt32 idxSize = 0;
  switch (indexType)
  {
  case eDT_UnsignedShort:
    idxSize = sizeof(csUInt16);
    break;
  case eDT_UnsignedInt:
    idxSize = sizeof(csUInt32);
    break;
  default:
    printf("Invalid index data type: %d %s:%s\n", indexType, locator.GetResourceFile().c_str(), locator.GetResourceName().c_str());
    subMesh->Release();
    return false;
  }
  unsigned char *buffer = new unsigned char[numIndices * idxSize];
  file->Read(buffer, numIndices * idxSize);
  iIndexBuffer *ib = csEng->CreateIndexBuffer(numIndices * idxSize, buffer, eBDM_Static);
  delete[] buffer;
  if (!ib)
  {
    subMesh->Release();
    return false;
  }

  subMesh->SetIndexBuffer(ib, numIndices, 0);

  csVector3f bboxMin;
  csVector3f bboxMax;
  file->Read(&bboxMin, sizeof(float) * 3);
  file->Read(&bboxMax, sizeof(float) * 3);
  csBoundingBox bbox;
  bbox.Add(bboxMin);
  bbox.Add(bboxMax);
  bbox.Finish();
  subMesh->SetBoundingBox(bbox);

  mesh->AddMesh(subMesh, materialIndex, LOD);

  return true;
}


iVertexDeclaration *csStaticMeshLoader::ReadVertexDeclaration(iFile *file) const
{
  csUInt32 numVertexDeclarations;
  file->Read(&numVertexDeclarations, sizeof(csUInt32));

  std::vector<csVertexElement> elements;
  for (csUInt32 i = 0; i < numVertexDeclarations; ++i)
  {
    csUInt32 data[6];
    file->Read(data, sizeof(csUInt32) * 6);

    elements.push_back(csVertexElement(
      (csVertexStreamType)data[0],
      (csDataType)data[1],
      data[2],
      data[3],
      data[4],
      data[5]
      ));
  }
  elements.push_back(csVertexElement());

  iVertexDeclaration *decl = csEng->CreateVertexDeclaration(elements.data());

  return decl;
}


csPhysicsShapeContainer *csStaticMeshLoader::ReadCollision(csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  csPhysicsShapeContainer *container = new csPhysicsShapeContainer();
  csUInt32 numShapes;
  file->Read(&numShapes, sizeof(csUInt32));
  for (csUInt32 i = 0; i < numShapes; ++i)
  {
    csUInt32 shapeTypeI;
    file->Read(&shapeTypeI, sizeof(csUInt32));
    csMatrix4f trans;
    file->Read(&trans, sizeof(csMatrix4f));
    csPhysGeometryType type = static_cast<csPhysGeometryType>(shapeTypeI);
    switch (type)
    {
    case ePGT_Box:
      {
        csPhysGeometry geom;
        memset(&geom, 0, sizeof(geom));
        geom.Type = type;
        file->Read(&geom.Dimensions, sizeof(csVector3f));
        iPhysicsShape *shape = csEng->CreateShape(geom);
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


csGeometryData* csStaticMeshLoader::ReadGeometry(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  csUInt32 type;
  file->Read(&type, sizeof(csUInt32));

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


csGeometryMesh* csStaticMeshLoader::ReadGeometryMesh(std::map<std::string, HeaderEntry> &entries, csUInt32 fileVersion, iFile *file, const csResourceLocator &locator, iObject *userData) const
{
  enum ReadMode
  {
    eRM_Internal,
    eRM_External
  };
  csMatrix4f localTransformation;
  file->Read(&localTransformation, sizeof(float) * 16);

  csUInt32 readMode;
  file->Read(&readMode, sizeof(csUInt32));

  iObject *meshObj = 0;
  switch (readMode)
  {
  case eRM_Internal:
    {
      std::string name = ReadString(file);
      csSize currentPosition = file->Tell();
      meshObj = ReadEntry(entries, name, fileVersion, file, locator, userData);
      file->Seek(eSP_Set, (long)currentPosition);
    }
    break;
  case eRM_External:
    {
      std::string resourceFile = ReadString(file);
      std::string resourceName = ReadString(file);
      meshObj = csResourceManager::Get()->GetOrLoad(csResourceLocator(resourceFile, resourceName), userData);
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

  csMesh *mesh = csQueryClass<csMesh>(meshObj);
  if (!mesh)
  {
    meshObj->Release();
    return 0;
  }

  csMultiMaterial *material = ReadMultiMaterial(file);


  csGeometryMesh *geometryMesh = new csGeometryMesh();
  geometryMesh->SetMesh(mesh);
  geometryMesh->SetMaterial(material);

  return geometryMesh;
}

csMultiMaterial *csStaticMeshLoader::ReadMultiMaterial(iFile *file) const
{
  csUInt32 numberOfMaterials;
  file->Read(&numberOfMaterials, sizeof(csUInt32));

  csResourceManager *mgr = csResourceManager::Get();
  csMultiMaterial *multiMaterial = new csMultiMaterial();
  for (csUInt32 i = 0; i < numberOfMaterials; ++i)
  {
    std::string name = ReadString(file);
    csMaterial *inst = mgr->GetOrLoad<csMaterial>(csResourceLocator("${materials}/materials.xml", name));
    if (inst)
    {
      multiMaterial->AddMaterialInstance(inst);
    }
  }
  return multiMaterial;
}

std::string csStaticMeshLoader::ReadString(iFile *file) const
{
  csUInt32 length;
  file->Read(&length, sizeof(csUInt32));

  if (length == 0)
  {
    return std::string();
  }

  char *buffer = new char[length];
  file->Read(buffer, length);
  std::string res(buffer);
  delete[] buffer;
  return res;
}





