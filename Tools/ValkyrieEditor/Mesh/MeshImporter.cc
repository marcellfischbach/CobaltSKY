
#include <Mesh/MeshImporter.hh>
#include <Mesh/Assimp.hh>
#include <AssetManager/AssetWriter.hh>
#include <Valkyrie/Core/AssetStream.hh>
#include <Valkyrie/Core/IFile.hh>
#include <Valkyrie/Core/VFS.hh>
#include <Valkyrie/Defs.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Math/BoundingVolume.hh>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <qdir.h>
#include <qdom.h>

namespace mesh
{

struct Mesh
{
  vkMatrix4f matrix;
  aiMesh *mesh;
};

struct Shape
{
  vkMatrix4f matrix;
  aiMesh *mesh;
  aiNode *node;
};

struct Bone
{
  vkString name;
  vkMatrix4f matrix;
  std::vector<Bone> childBones;
};


static void Debug(const aiScene* scene);
static void Debug(const aiScene *scene, aiNode* node, int i);
static void Debug(aiMesh* mesh);
static void CollectData(std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, Bone &skeleton, std::vector<Shape> &collisions, const aiScene *scene);
static void CollectMeshes(std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix);
static void CollectSkeleton(Bone &skeleton, const aiScene *scene, aiNode *node);
static void CollectCollisions(std::vector<Shape> &collisions, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix);
static QDomElement CreateSkeletonElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, Bone &bone, const QString &dataName);
static QDomElement CreateMeshElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, const QString &dataName);
static QDomElement CreateShapesElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, std::vector<Shape> &collisions, const QString &dataName);
static void WriteMeshToOutputStream(vkAssetOutputStream &outputStream, const vkMatrix4f &matrix, const vkMatrix4f &normalMatrix, aiMesh *mesh);
static bool ExportEntity(const aiScene *scene, 
                         bool meshes, const QString &meshSuffix, std::vector<unsigned> &materialIndices,
                         bool skeleton, const QString &skeletonSuffix, 
                         bool collisions, const QString &collisionsSuffix, 
                         const QString &dataPath, const QString &dataName);
static vkString ValidateName(const vkString &name);

Importer *Importer::Get()
{
  static Importer importer;
  return &importer;
}

bool Importer::CanImport(const QFileInfo &info)
{
  QString fileName = info.fileName();
  return fileName.endsWith(".dae")
    || fileName.endsWith(".fbx");
}

bool contains(const std::vector<unsigned> &indices, unsigned idx)
{
  for (unsigned i = 0, in = indices.size(); i < in; ++i)
  {
    if (indices[i] == idx)
    {
      return true;
    }
  }
  return false;
}

unsigned indexOf(const std::vector<unsigned> &indices, unsigned idx)
{
  for (unsigned i = 0, in = indices.size(); i < in; ++i)
  {
    if (indices[i] == idx)
    {
      return i;
    }
  }
  return 0;
}


bool Importer::Import(const QFileInfo &info, const QDir &outputDir)
{
  Assimp::Importer *importer = AssimpImporterFactory::CreateImporter();
  if (!importer)
  {
    return false;
  }


  // eval the relative path within the VFS:
  QString fileName = info.absoluteFilePath();
  QFileInfo rootPathInfo(vkVFS::Get()->GetRootPath().c_str());
  QString rootPath = rootPathInfo.absoluteFilePath();
  QString dataPath = outputDir.absolutePath();
  if (dataPath.startsWith(rootPath))
  {
    dataPath = dataPath.right(dataPath.length() - rootPath.length() - 1);
    if (dataPath.length() > 0 && dataPath[0] == '/')
    {
      dataPath = dataPath.right(-1);
    }
  }

  QString xassetFileName = dataPath + "/" + info.baseName() + ".xasset";
  QString dataFileName = dataPath + "/" + info.baseName() + ".data";



  const aiScene *scene = importer->ReadFile((const char*)fileName.toLatin1(), 0
                                            | aiProcess_CalcTangentSpace
                                            | aiProcess_JoinIdenticalVertices
                                            //| aiProcess_MakeLeftHanded
                                            | aiProcess_Triangulate
                                            | aiProcess_LimitBoneWeights
                                            | aiProcess_FlipWindingOrder
  );
  if (!scene)
  {
    delete importer;
    return false;
  }



  std::vector<Mesh> meshes;
  std::vector<Shape> collisions;
  std::vector<unsigned> materialIndices;
  Bone skeleton;
  skeleton.name = "undefined";
  CollectData(meshes, materialIndices, skeleton, collisions, scene);

  bool hasMesh = false;
  bool hasCollider = false;
  bool hasSkeleton = false;

  unsigned numDatas = 0;
  if (meshes.size() > 0)
  {
    hasMesh = true;
    numDatas++;
  }
  if (skeleton.name != vkString("undefined"))
  {
    hasSkeleton = true;
    numDatas++;
  }
  if (collisions.size() > 0)
  {
    hasCollider = true;
    numDatas++;
  }


  assetmanager::AssetWriter writer;

  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement entityStateElement = doc.createElement("entityState");


  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(entityStateElement);

  // maybe make that a skinned mesh state later when there is a skeleton attached
  entityStateElement.setAttribute("name", info.baseName());
  entityStateElement.setAttribute("class", "vkStaticMeshState");

  if (hasMesh)
  {
    QDomElement meshElement = CreateMeshElement(doc, writer, scene, meshes, materialIndices, dataFileName);
    entityStateElement.appendChild(meshElement);

    QDomElement materialsElement = doc.createElement("materials");
    entityStateElement.appendChild(materialsElement);

    for (size_t i = 0, in = materialIndices.size(); i < in; ++i)
    {
      aiString materialName;
      scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_NAME, materialName);

      QDomElement materialElement = doc.createElement("material");
      materialElement.setAttribute("slot", i);
      materialElement.appendChild(doc.createTextNode("materials/" + QString(materialName.C_Str()) + ".xasset"));
      materialsElement.appendChild(materialElement);
    }
  }

  if (hasCollider)
  {
    QDomElement colliderElement = doc.createElement("collider");
    QDomElement shapesElement = CreateShapesElement(doc, writer, scene, collisions, dataFileName);
    colliderElement.appendChild(shapesElement);
    colliderElement.appendChild(doc.createElement("friction")).appendChild(doc.createTextNode("10.0"));
    colliderElement.appendChild(doc.createElement("restitution")).appendChild(doc.createTextNode("0.5"));
    entityStateElement.appendChild(colliderElement);
  }

  QString xmlDoc = doc.toString(2);
  IFile *xassetOutputFile = vkVFS::Get()->Open(vkString((const char*)xassetFileName.toLatin1()), eOM_Write, eTM_Binary);
  if (!xassetOutputFile)
  {
    return false;
  }
  xassetOutputFile->Write((const char*)xmlDoc.toLatin1(), xmlDoc.length());
  xassetOutputFile->Close();

  IFile *dataOutputFile = vkVFS::Get()->Open(vkString((const char*)dataFileName.toLatin1()), eOM_Write, eTM_Binary);
  if (!dataOutputFile)
  {
    return false;
  }
  writer.Output(dataOutputFile);
  dataOutputFile->Close();
}

void ExportBone(Bone &bone, QDomElement parentElement, QDomDocument doc)
{
  QDomElement boneElement = doc.createElement("bone");
  boneElement.setAttribute("name", bone.name.c_str());
  parentElement.appendChild(boneElement);


  for (size_t i=0, in = bone.childBones.size(); i<in; ++i)
  {
    Bone &child = bone.childBones[i];
    ExportBone(child, boneElement, doc);
  }
}

QDomElement CreateSkeletonElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, Bone &bone)
{
  QDomElement skeletonElement = doc.createElement("skeleton");


  for (Bone &b : bone.childBones)
  {
    ExportBone(b, skeletonElement, doc);
  }

  return skeletonElement;
}

QDomElement CreateMeshElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, const QString &dataName)
{

  QDomElement meshElement = doc.createElement("mesh");
  QDomElement materialSlotsElement = doc.createElement("materialSlots");
  QDomElement globalIndicesElement = doc.createElement("globalIndices");
  QDomElement subMeshesElement = doc.createElement("subMeshes");

  meshElement.appendChild(materialSlotsElement);
  meshElement.appendChild(globalIndicesElement);
  meshElement.appendChild(subMeshesElement);


  //
  // export the material names 

  for (unsigned i = 0, in = materialIndices.size(); i < in; ++i)
  {
    aiString materialName;
    scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_NAME, materialName);

    QDomElement materialSlotElement = doc.createElement("materialSlot");
    materialSlotElement.setAttribute("id", i);
    materialSlotElement.setAttribute("name", materialName.C_Str());
    materialSlotsElement.appendChild(materialSlotElement);
  }


  // we don't use global index buffers here
  // later use global index buffers here


  // write the submeshes
  for (Mesh &mesh : meshes)
  {
    QDomElement subMeshElement = doc.createElement("subMesh");
    subMeshElement.setAttribute("lod", 0); // <- there is currently only LOD 0
    subMeshElement.setAttribute("name", mesh.mesh->mName.C_Str());
    subMeshElement.setAttribute("materialSlot", indexOf(materialIndices, mesh.mesh->mMaterialIndex));
    subMeshElement.appendChild(doc.createTextNode(dataName + ":" + ValidateName(mesh.mesh->mName.C_Str()).c_str()));

    subMeshesElement.appendChild(subMeshElement);

    vkAssetOutputStream outputStream;

    vkMatrix4f normalMatrix;
    //transpose(inverse(modelView))
    mesh.matrix.Inverted(normalMatrix);
    normalMatrix.Transpose();
    WriteMeshToOutputStream(outputStream, mesh.matrix, normalMatrix, mesh.mesh);

    writer.AddEntry(ValidateName(vkString(mesh.mesh->mName.C_Str())), "SUBMESH", outputStream.GetSize(), outputStream.GetBuffer());
  }

  return meshElement;
}


vkVector3f EvalSize(aiMesh *mesh)
{
  vkVector3f r(0.0f, 0.0f, 0.0f);
  for (size_t i = 0; i < mesh->mNumVertices; ++i)
  {
    aiVector3D &aiv = mesh->mVertices[i];
    vkVector3f v(abs(aiv.x), abs(aiv.y), abs(aiv.z));
    r.x = r.x > v.x ? r.x : v.x;
    r.y = r.y > v.y ? r.y : v.y;
    r.z = r.z > v.z ? r.z : v.z;
  }

  return r;
}

QDomElement CreateShapesElement(QDomDocument doc, assetmanager::AssetWriter &writer, const aiScene *scene, std::vector<Shape> &collisions, const QString &dataName)
{

  QDomElement shapesElement = doc.createElement("shapes");


  for (Shape &collision : collisions)
  {
    vkString name(collision.node->mName.C_Str());
    QDomElement shapeElement = doc.createElement("shape");
    shapeElement.setAttribute("name", name.c_str());

    QDomElement transformElement = doc.createElement("transform");
    shapeElement.appendChild(transformElement);
    QDomElement matrixElement = doc.createElement("matrix");
    transformElement.appendChild(matrixElement);

    const vkMatrix4f &m = collision.matrix;
    matrixElement.appendChild(doc.createElement("row0")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m00, m.m01, m.m02, m.m03)));
    matrixElement.appendChild(doc.createElement("row1")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m10, m.m11, m.m12, m.m13)));
    matrixElement.appendChild(doc.createElement("row2")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m20, m.m21, m.m22, m.m23)));
    matrixElement.appendChild(doc.createElement("row3")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m30, m.m31, m.m32, m.m33)));
    

    vkVector3f size = EvalSize(collision.mesh);
    if (name.length() >= 6 && name.substr(0, 6) == vkString("COLBOX"))
    {
      QDomElement boxElement = doc.createElement("box");
      boxElement.setAttribute("halfX", size.x);
      boxElement.setAttribute("halfY", size.y);
      boxElement.setAttribute("halfZ", size.z);
      shapeElement.appendChild(boxElement);
    }
    else if (name.length() >= 6 && name.substr(0, 6) == vkString("COLSPH"))
    {
      float r = size.x > size.y ? size.x : size.y;
      r = r > size.z ? r : size.z;
      QDomElement sphereElement = doc.createElement("sphere");
      sphereElement.setAttribute("radius", r);
      shapeElement.appendChild(sphereElement);
    }
    else if (name.length() >= 6 && name.substr(0, 6) == vkString("COLCYL"))
    {
      QDomElement cylinderElement = doc.createElement("cylinder");
      float r = size.x > size.y ? size.x : size.y;
      cylinderElement.setAttribute("radius", r);
      cylinderElement.setAttribute("halfHeight", size.z);
      shapeElement.appendChild(cylinderElement);
    }
    else if (name.length() >= 6 && name.substr(0, 6) == vkString("COLTRI"))
    {
      QDomElement triMeshElement = doc.createElement("triMesh");
      triMeshElement.appendChild(doc.createTextNode(dataName + ":" + ValidateName(name).c_str()));

      shapeElement.appendChild(triMeshElement);
    }
    else if (name.length() >= 6 && name.substr(0, 6) == vkString("COLHUL"))
    {
      QDomElement convexHullElement = doc.createElement("convexHull");
      convexHullElement.appendChild(doc.createTextNode(dataName + ":" + ValidateName(name).c_str()));
      shapeElement.appendChild(convexHullElement);
    }
    else
    {
      continue;
    }
    shapesElement.appendChild(shapeElement);
  }

  return shapesElement;
}


bool ExportEntity(const aiScene *scene, 
                  bool meshes, const QString &meshSuffix, std::vector<unsigned> &materialIndices,
                  bool skeleton, const QString &skeletonSuffix, 
                  bool collisions, const QString &collisionsSuffix, 
                  const QString &dataPath, const QString &dataName)
{
  QDomDocument doc;

  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement entityElement = doc.createElement("entity");
  QDomElement entityStatesElement = doc.createElement("entityStates");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(entityElement);
  entityElement.appendChild(entityStatesElement);

  entityElement.setAttribute("name", dataName);
  int nextID = 0;
  int collisionID = -1;
  int meshesID = -1;
  if (collisions)
  {
    collisionID = nextID++;

    QDomElement staticColliderStateElement = doc.createElement("entityState");
    staticColliderStateElement.setAttribute("class", "vkStaticColliderState");
    staticColliderStateElement.setAttribute("id", collisionID);

    staticColliderStateElement.appendChild(doc.createElement("shape")).appendChild(doc.createTextNode(dataPath + "/" + dataName + collisionsSuffix + ".xasset"));
    staticColliderStateElement.appendChild(doc.createElement("friction")).appendChild(doc.createTextNode("10.0"));
    staticColliderStateElement.appendChild(doc.createElement("restitution")).appendChild(doc.createTextNode("0.5"));

    entityStatesElement.appendChild(staticColliderStateElement);
  }

  if (meshes)
  {
    meshesID = nextID++;
    QDomElement staticMeshStateElement = doc.createElement("entityState");
    staticMeshStateElement.setAttribute("class", "vkStaticMeshState");
    staticMeshStateElement.setAttribute("id", meshesID);
    if (collisionID != -1)
    {
      staticMeshStateElement.setAttribute("parentID", collisionID);
    }

    staticMeshStateElement.appendChild(doc.createElement("mesh")).appendChild(doc.createTextNode(dataPath + "/" + dataName + meshSuffix + ".xasset"));

    QDomElement materialsElement = doc.createElement("materials");
    staticMeshStateElement.appendChild(materialsElement);

    for (size_t i = 0, in = materialIndices.size(); i < in; ++i)
    {
      aiString materialName;
      scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_NAME, materialName);

      QDomElement materialElement = doc.createElement("material");
      materialElement.setAttribute("slot", i);
      materialElement.appendChild(doc.createTextNode("materials/" + QString(materialName.C_Str()) + ".xasset"));
      materialsElement.appendChild(materialElement);
    }

    entityStatesElement.appendChild(staticMeshStateElement);
  }


  QDomElement rootStateElement = doc.createElement("rootState");
  rootStateElement.setAttribute("id", 0);

  entityElement.appendChild(rootStateElement);


  QString xAssetFileName = dataPath + "/" + dataName + ".xasset";

  QString xml = doc.toString(2);
  IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)xAssetFileName.toLatin1()), eOM_Write, eTM_Text);
  if (!finalOutputFile)
  {
    return false;
  }
  finalOutputFile->Write((const char*)xml.toLatin1(), xml.length());
  finalOutputFile->Close();
  return true;
}


vkString ValidateName(const vkString &name)
{
  vkString res = name;
  for (vkSize i = 0, in = res.length(); i < in; ++i)
  {
    char ch = res[i];
    if (ch != ':' && ch != '.')
    {
      continue;
    }

    res[i] = '_';
  }
  return res;
}


void WriteMeshToOutputStream(vkAssetOutputStream &outputStream, const vkMatrix4f &matrix, const vkMatrix4f &normalMatrix, aiMesh *mesh)
{
  outputStream
    << (vkUInt32)VK_VERSION(1, 0, 0)
    << (vkUInt32)ePT_Triangles
    << (vkUInt32)(mesh->mNumVertices >= 65536 ? eDT_UnsignedInt : eDT_UnsignedShort);

  vkBoundingBox bbox;
  //
  // output the vertex declarations
  vkUInt8 numVertexDeclarations = 0;
  vkSize stride = 0;
  if (mesh->HasPositions())
  {
    numVertexDeclarations++;
    stride += sizeof(float) * 3;
  }
  if (mesh->HasNormals())
  {
    numVertexDeclarations++;
    stride += sizeof(float) * 3;
  }
  if (mesh->HasTangentsAndBitangents())
  {
    numVertexDeclarations+=2;
    stride += sizeof(float) * 6;
  }
  for (unsigned i = 0; i < mesh->GetNumUVChannels(); ++i)
  {
    numVertexDeclarations++;
    stride += sizeof(float) * mesh->mNumUVComponents[i];
  }

  if (mesh->GetNumColorChannels() > 0)
  {
    numVertexDeclarations++;
    stride += sizeof(float) * 4;
  }

  outputStream << numVertexDeclarations;

  // output all one single vertex buffer
  vkUInt32 offset = 0;
  if (mesh->HasPositions())
  {
    outputStream
      << (vkUInt32)eVST_Position
      << (vkUInt32)eDT_Float
      << (vkSize)3
      << offset
      << stride
      << (vkUInt8)0;
    offset += 3 * sizeof(float);
  }
  if (mesh->HasNormals())
  {
    outputStream
      << (vkUInt32)eVST_Normal
      << (vkUInt32)eDT_Float
      << (vkSize)3
      << offset
      << stride
      << (vkUInt8)0;
    offset += 3 * sizeof(float);
  }
  if (mesh->HasTangentsAndBitangents())
  {
    outputStream
      << (vkUInt32)eVST_Tangent
      << (vkUInt32)eDT_Float
      << (vkSize)3
      << offset
      << stride
      << (vkUInt8)0;
    offset += 3 * sizeof(float);
    outputStream
      << (vkUInt32)eVST_BiNormal
      << (vkUInt32)eDT_Float
      << (vkSize)3
      << offset
      << stride
      << (vkUInt8)0;
    offset += 3 * sizeof(float);
  }
  for (unsigned i = 0; i < mesh->GetNumUVChannels(); ++i)
  {
    outputStream
      << (vkUInt32)eVST_TexCoord0 + i
      << (vkUInt32)eDT_Float
      << (vkSize)mesh->mNumUVComponents[i]
      << offset
      << stride
      << (vkUInt8)0;
    offset += mesh->mNumUVComponents[i] * sizeof(float);
  }
  if (mesh->GetNumColorChannels() > 0)
  {
    outputStream
      << (vkUInt32)eVST_Color
      << (vkUInt32)eDT_Float
      << (vkSize)4
      << offset
      << stride
      << (vkUInt8)0;
    offset += 4 * sizeof(float);
  }

  //
  // now output the vertices
  outputStream 
    << (vkUInt8)1 // just one stream as stated above
    << (vkSize)(mesh->mNumVertices * stride);
  for (unsigned i = 0; i < mesh->mNumVertices; ++i)
  {
    if (mesh->HasPositions())
    {
      aiVector3D &v = mesh->mVertices[i];
      vkVector3f vf(v.x, v.y, v.z);
      vkMatrix4f::Transform(matrix, vf, vf);
      bbox.Add(vf);
      outputStream << vf;
    }
    if (mesh->HasNormals())
    {
      aiVector3D &v = mesh->mNormals[i];
      vkVector3f vf(v.x, v.y, v.z);
      vkMatrix4f::Mult(normalMatrix, vf, vf);
      outputStream << vf;
    }
    if (mesh->HasTangentsAndBitangents())
    {
      aiVector3D &v = mesh->mTangents[i];
      vkVector3f vf(v.x, v.y, v.z);
      vkMatrix4f::Mult(normalMatrix, vf, vf);
      outputStream << vf;
      v = mesh->mBitangents[i];
      vf.Set(-v.x, -v.y, -v.z);
      vkMatrix4f::Mult(normalMatrix, vf, vf);
      outputStream << vf;
    }
    for (unsigned j = 0, jn = mesh->GetNumUVChannels(); j < jn; ++j)
    {
      for (unsigned k = 0, kn = mesh->mNumUVComponents[j]; k < kn; ++k)
      {
        outputStream << (float)mesh->mTextureCoords[j][i].v[k];
      }
    }
    if (mesh->GetNumColorChannels() > 0)
    {
      aiColor4D &col = mesh->mColors[0][i];
      outputStream << col.r << col.g << col.b << col.a;
    }
  }

  bbox.Finish();

  //
  // output the indices ... discriminate between 16 and 32 bit indices
  vkSize count = mesh->mNumFaces * 3;
  outputStream
    << (vkInt8)-1 // we don't use a shared index buffer ... we provide a real one here
    << count
    << (vkSize)0;

  if (mesh->mNumVertices >= 65536)
  {
    outputStream << (vkSize)count * sizeof(vkUInt32);
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace &face = mesh->mFaces[i];
      outputStream
        << (vkUInt32)face.mIndices[0]
        << (vkUInt32)face.mIndices[1]
        << (vkUInt32)face.mIndices[2];
    }
  }
  else
  {
    outputStream << (vkSize)count * sizeof(vkUInt16);
    for (unsigned i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace &face = mesh->mFaces[i];
      outputStream
        << (vkUInt16)face.mIndices[0]
        << (vkUInt16)face.mIndices[1]
        << (vkUInt16)face.mIndices[2];
    }
  }


  // 
  // and finaly output a bounding box
  outputStream << bbox.GetMin() << bbox.GetMax();
}


int indent = 0;
void CollectData(std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, Bone &skeleton, std::vector<Shape> &collisions, const aiScene *scene)
{
  vkMatrix4f parentMatrix;
  aiNode *root = scene->mRootNode;

  for (unsigned i = 0, in = root->mNumChildren; i < in; ++i)
  {
    aiNode *child = root->mChildren[i];
    indent++;
    vkString name = vkString(child->mName.C_Str());
    if (name.length () >= 8 && (name.substr(0, 8) == vkString("Skeleton") || name.substr(0, 8) == vkString("Armature")))
    {
      CollectSkeleton(skeleton, scene, child);
    }
    else if (name.length() >= 3 && (name.substr(0, 3) == vkString("COL")))
    {
      CollectCollisions(collisions, scene, child, parentMatrix);
    }
    else
    {
      CollectMeshes(meshes, materialIndices, scene, child, parentMatrix);
    }
    indent--;
  }
}

void CollectMeshes(std::vector<Mesh> &meshes, std::vector<unsigned> &materialIndices, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix)
{
  vkMatrix4f M(node->mTransformation.mData);
  M.Transpose();
  M = vkMatrix4f::Mult(parentMatrix, M, M);
  for (unsigned i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh m;
    m.mesh = mesh;
    m.matrix = M;
    meshes.push_back(m);


    if (!contains(materialIndices, mesh->mMaterialIndex))
    {
      materialIndices.push_back(mesh->mMaterialIndex);
    }
  }
  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];
    CollectMeshes(meshes, materialIndices, scene, child, M);
  }
}

void CollectCollisions(std::vector<Shape> &collisions, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix)
{
  vkMatrix4f M(node->mTransformation.mData);
  M.Transpose();
  M = vkMatrix4f::Mult(parentMatrix, M, M);
  for (unsigned i = 0; i < node->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    Shape s;
    s.mesh = mesh;
    s.matrix = M;
    s.node = node;
    collisions.push_back(s);
  }
  
  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];
    CollectCollisions(collisions, scene, child, M);
  }
}


void CollectBone(Bone &bone, const aiScene *scene, aiNode *node)//, const vkMatrix4f &parentMatrix)
{
  vkMatrix4f M(node->mTransformation.mData);
  M.Transpose();
  bone.matrix = M;
  bone.name = node->mName.C_Str();

  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];

    // add a new bone but work with a reference on the real entry
    Bone nbone;
    bone.childBones.push_back(nbone);
    Bone &boneRef = *(bone.childBones.rbegin());

    CollectBone(boneRef, scene, child);

  }

}

void CollectSkeleton(Bone &skeleton, const aiScene *scene, aiNode *node)
{
  vkMatrix4f M(node->mTransformation.mData);
  M.Transpose();
  skeleton.matrix = M;
  skeleton.name = "Skeleton";


  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];

    // add a new bone but work with a reference on the real entry
    Bone bone;
    skeleton.childBones.push_back(bone);
    Bone &boneRef = *(skeleton.childBones.rbegin());

    CollectBone(boneRef, scene, child);

  }
}



void Debug(const aiScene *scene)
{
  Debug(scene, scene->mRootNode, 0);
  for (unsigned i = 0; i < scene->mNumMeshes; ++i)
  {
    Debug(scene->mMeshes[i]);
  }
}

void ind(int indent)
{
  for (unsigned i = 0; i < indent; ++i)
  {
    printf("  ");
  }
}

void Debug(const aiScene *scene, aiNode *node, int indent)
{
  ind(indent);
  printf("Node: %s\n", node->mName.C_Str());

  ind(indent); printf("  %f %f %f %f\n", node->mTransformation.mData[0], node->mTransformation.mData[1], node->mTransformation.mData[2], node->mTransformation.mData[3]);
  ind(indent); printf("  %f %f %f %f\n", node->mTransformation.mData[4], node->mTransformation.mData[5], node->mTransformation.mData[6], node->mTransformation.mData[7]);
  ind(indent); printf("  %f %f %f %f\n", node->mTransformation.mData[8], node->mTransformation.mData[9], node->mTransformation.mData[10], node->mTransformation.mData[11]);
  ind(indent); printf("  %f %f %f %f\n", node->mTransformation.mData[12], node->mTransformation.mData[13], node->mTransformation.mData[14], node->mTransformation.mData[15]);

  for (unsigned i = 0; i < node->mNumMeshes; ++i)
  {
    ind(indent); printf("  Mesh: [%d] \"%s\"\n", node->mMeshes[i], scene->mMeshes[node->mMeshes[i]]->mName.C_Str());
  }

  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    Debug(scene, node->mChildren[i], indent + 1);
  }
}

void Debug(aiMesh *mesh)
{
  printf("  Mesh: %s\n", mesh->mName.C_Str());
  printf("    Vertices: %d\n", mesh->mNumVertices);
  for (unsigned i = 0; i < mesh->mNumVertices; ++i)
  {
    aiVector3D &vertex = mesh->mVertices[i];
    aiVector3D &texCoord = mesh->mTextureCoords[0][i];
    aiVector3D &normal = mesh->mNormals[i];
    aiVector3D &tangent = mesh->mTangents[i];
    aiVector3D &bitangent = mesh->mBitangents[i];

    printf("    %d  <%.2f %.2f %.2f>  <%.2f %.2f>  <%.2f %.2f %.2f>  <%.2f %.2f %.2f>  <%.2f %.2f %.2f>\n", i, vertex.x, vertex.y, vertex.z, texCoord.x, texCoord.y, normal.x, normal.y, normal.z, tangent.x, tangent.y, tangent.z, bitangent.x, bitangent.y, bitangent.z);
  }
}


}