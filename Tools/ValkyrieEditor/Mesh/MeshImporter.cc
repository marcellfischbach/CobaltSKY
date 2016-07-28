
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
static void CollectData(std::vector<Mesh> &meshes, Bone &skeleton, std::vector<Shape> &collisions, const aiScene *scene);
static void CollectMeshes(std::vector<Mesh> &meshes, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix);
static void CollectSkeleton(Bone &skeleton, const aiScene *scene, aiNode *node);
static void CollectCollisions(std::vector<Shape> &collisions, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix);
static bool ExportSkeleton(const aiScene *scene, Bone &bone, const QString &dataPath, const QString &dataName, const QString &suffix);
static bool ExportMeshes(const aiScene *scene, std::vector<Mesh> &meshes, const QString &dataPath, const QString &dataName, const QString &suffix);
static bool ExportCollisions(const aiScene *scene, std::vector<Shape> &collisions, const QString &dataPath, const QString &dataName, const QString &suffix);
static void WriteMeshToOutputStream(vkAssetOutputStream &outputStream, const vkMatrix4f &matrix, const vkMatrix4f &normalMatrix, aiMesh *mesh);
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
  Bone skeleton;
  skeleton.name = "undefined";
  CollectData(meshes, skeleton, collisions, scene);


  unsigned numDatas = 0;
  if (meshes.size() > 0)
  {
    numDatas++;
  }
  if (skeleton.name != vkString("undefined"))
  {
    numDatas++;
  }
  if (collisions.size() > 0)
  {
    numDatas++;
  }

  bool multi = numDatas > 1;
  if (multi)
  {
    QString dataParentPath = rootPath + "/" + dataPath;
    dataPath += "/" + info.baseName() + ".fasset";

    QDir dir(dataParentPath);
    dir.mkdir(info.baseName() + ".fasset");
  }



  if (meshes.size() > 0)
  {
    ExportMeshes(scene, meshes, dataPath, info.baseName(), multi ? "_Mesh" : "");
  }
  if (skeleton.name != vkString("undefined"))
  {
    ExportSkeleton(scene, skeleton, dataPath, info.baseName(), multi ? "_Skeleton" : "");
  }
  if (collisions.size() > 0)
  {
    ExportCollisions(scene, collisions, dataPath, info.baseName(), multi ? "_Collision" : "");
  }
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

bool  ExportSkeleton(const aiScene *scene, Bone &bone, const QString &dataPath, const QString &dataName, const QString &suffix)
{
  QString xAssetFileName = dataPath + "/" + dataName + suffix + ".xasset";
  QString dataFileName = dataPath + "/" + dataName + suffix + ".data";

  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement skeletonElement = doc.createElement("skeleton");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(skeletonElement);

  for (Bone &b : bone.childBones)
  {
    ExportBone(b, skeletonElement, doc);
  }
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

bool ExportMeshes(const aiScene *scene, std::vector<Mesh> &meshes, const QString &dataPath, const QString &dataName, const QString &suffix)
{

  QString xAssetFileName = dataPath + "/" + dataName + suffix + ".xasset";
  QString dataFileName = dataPath + "/" + dataName + suffix + ".data";

  assetmanager::AssetWriter writer;


  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement meshElement = doc.createElement("mesh");
  QDomElement materialSlotsElement = doc.createElement("materialSlots");
  QDomElement globalIndicesElement = doc.createElement("globalIndices");
  QDomElement subMeshesElement = doc.createElement("subMeshes");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(meshElement);
  meshElement.appendChild(materialSlotsElement);
  meshElement.appendChild(globalIndicesElement);
  meshElement.appendChild(subMeshesElement);


  //
  // export the material names 
  std::vector<unsigned> materialIndices;
  for (Mesh &mesh : meshes)
  {
    if (!contains(materialIndices, mesh.mesh->mMaterialIndex))
    {
      materialIndices.push_back(mesh.mesh->mMaterialIndex);
    }
  }
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
    subMeshElement.appendChild(doc.createTextNode(dataFileName + ":" + ValidateName(mesh.mesh->mName.C_Str()).c_str()));

    subMeshesElement.appendChild(subMeshElement);

    vkAssetOutputStream outputStream;

    vkMatrix4f normalMatrix;
    //transpose(inverse(modelView))
    mesh.matrix.Inverted(normalMatrix);
    normalMatrix.Transpose();
    WriteMeshToOutputStream(outputStream, mesh.matrix, normalMatrix, mesh.mesh);

    writer.AddEntry(ValidateName(vkString(mesh.mesh->mName.C_Str())), "SUBMESH", outputStream.GetSize(), outputStream.GetBuffer());
  }




  IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)dataFileName.toLatin1()), eOM_Write, eTM_Binary);
  if (!finalOutputFile)
  {
    return false;
  }
  writer.Output(finalOutputFile);
  finalOutputFile->Close();



  QString xml = doc.toString(2);
  finalOutputFile = vkVFS::Get()->Open(vkString((const char*)xAssetFileName.toLatin1()), eOM_Write, eTM_Text);
  if (!finalOutputFile)
  {
    return false;
  }
  finalOutputFile->Write((const char*)xml.toLatin1(), xml.length());
  finalOutputFile->Close();


  return true;
}

vkVector3f EvalSize(aiMesh *mesh)
{
  vkVector3f r;
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

bool ExportCollisions(const aiScene *scene, std::vector<Shape> &collisions, const QString &dataPath, const QString &dataName, const QString &suffix)
{

  QString xAssetFileName = dataPath + "/" + dataName + suffix + ".xasset";
  QString dataFileName = dataPath + "/" + dataName + suffix + ".data";

  assetmanager::AssetWriter writer;


  QDomDocument doc;
  QDomElement assetElement = doc.createElement("asset");
  QDomElement dataElement = doc.createElement("data");
  QDomElement collisionElement = doc.createElement("collision");
  QDomElement shapesElement = doc.createElement("shapes");

  doc.appendChild(assetElement);
  assetElement.appendChild(dataElement);
  dataElement.appendChild(collisionElement);
  collisionElement.appendChild(shapesElement);



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
    matrixElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m00, m.m01, m.m02, m.m03)));
    matrixElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m10, m.m11, m.m12, m.m13)));
    matrixElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m20, m.m21, m.m22, m.m23)));
    matrixElement.appendChild(doc.createElement("row")).appendChild(doc.createTextNode(QString::asprintf("%f, %f, %f, %f", m.m30, m.m31, m.m32, m.m33)));
    

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
      triMeshElement.appendChild(doc.createTextNode(dataFileName + ":" + ValidateName(name).c_str()));

      shapeElement.appendChild(triMeshElement);
    }
    else if (name.length() >= 6 && name.substr(0, 6) == vkString("COLHUL"))
    {
      QDomElement convexHullElement = doc.createElement("convexHull");
      convexHullElement.appendChild(doc.createTextNode(dataFileName + ":" + ValidateName(name).c_str()));
      shapeElement.appendChild(convexHullElement);
    }
    else
    {
      continue;
    }
    shapesElement.appendChild(shapeElement);
  }



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
  normalMatrix.Debug("NormalMatrix");
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
void CollectData(std::vector<Mesh> &meshes, Bone &skeleton, std::vector<Shape> &collisions, const aiScene *scene)
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
      CollectMeshes(meshes, scene, child, parentMatrix);
    }
    indent--;
  }
}

void CollectMeshes(std::vector<Mesh> &meshes, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix)
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
  }
  for (unsigned i = 0; i < node->mNumChildren; ++i)
  {
    aiNode *child = node->mChildren[i];
    CollectMeshes(meshes, scene, child, M);
  }
}

void CollectCollisions(std::vector<Shape> &collisions, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix)
{
  vkMatrix4f M(node->mTransformation.mData);
  M.Transpose();
  M.Debug();
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