
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

namespace mesh
{

struct Mesh
{
  vkMatrix4f matrix;
  aiMesh *mesh;
};


void Debug(const aiScene* scene);
void Debug(const aiScene *scene, aiNode* node, int i);
void Debug(aiMesh* mesh);
void CollectMeshes(std::vector<Mesh> &meshes, const aiScene *scene);
void CollectMeshes(std::vector<Mesh> &meshes, const aiScene *scene, aiNode *node, const vkMatrix4f &parentMatrix);



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

  QString fileName = info.absoluteFilePath();

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
  CollectMeshes(meshes, scene);

  vkAssetOutputStream outputStream;

  vkUInt32 version = VK_VERSION(1, 0, 0);
  outputStream << version;



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
  outputStream << (vkUInt32)materialIndices.size();
  for (unsigned i = 0, in = materialIndices.size(); i < in; ++i)
  {
    aiString materialName;
    scene->mMaterials[materialIndices[i]]->Get(AI_MATKEY_NAME, materialName);
    outputStream << vkString(materialName.C_Str());
  }


  // we don't use global index buffers here
  vkUInt32 numIndexBuffers = 0;
  outputStream << numIndexBuffers;



  // write the submeshes
  outputStream << (vkUInt32)meshes.size();
  for (Mesh &mesh : meshes)
  {
    outputStream
      << (vkSize)indexOf(materialIndices, mesh.mesh->mMaterialIndex)
      << (vkUInt8)0 // no LOD at the moment
      << vkString(mesh.mesh->mName.C_Str()); // this might produce doublicates

    vkMatrix4f normalMatrix;
    //transpose(inverse(modelView))
    mesh.matrix.Inverted(normalMatrix);
    normalMatrix.Transpose();
    Write(outputStream, mesh.matrix, normalMatrix, mesh.mesh);


  }

  delete importer;

  assetmanager::AssetWriter writer;
  writer.AddEntry("MESH", "DATA", outputStream.GetSize(), outputStream.GetBuffer());

  fileName = info.completeBaseName() + ".asset";
  QString absFileName = outputDir.absoluteFilePath(fileName);


  QFileInfo rootPathInfo(vkVFS::Get()->GetRootPath().c_str());
  QString rootPath = rootPathInfo.absoluteFilePath();

  if (absFileName.startsWith(rootPath, Qt::CaseInsensitive))
  {
    absFileName = absFileName.right(absFileName.length() - rootPath.length());
  }

  IFile *finalOutputFile = vkVFS::Get()->Open(vkString((const char*)absFileName.toLatin1()), eOM_Write, eTM_Binary);
  if (finalOutputFile)
  {
    writer.Output(finalOutputFile);
    finalOutputFile->Close();
    return true;
  }



  return false;
}

void Importer::Write(vkAssetOutputStream &outputStream, const vkMatrix4f &matrix, const vkMatrix4f &normalMatrix, aiMesh *mesh)
{
  outputStream
    << (vkUInt32)ePT_Triangles
    << (vkUInt32)(mesh->mNumVertices >= 65536 ? eDT_UnsignedInt : eDT_UnsignedShort);

  vkBoundingBox bbox;
  //
  // output the vertex declarations
  vkUInt8 numVertexDeclarations = 0;
  vkUInt32 stride = 0;
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
      vf.Set(v.x, v.y, v.z);
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



void CollectMeshes(std::vector<Mesh> &meshes, const aiScene *scene)
{
  vkMatrix4f parentMatrix;
  aiNode *root = scene->mRootNode;

  for (unsigned i = 0, in = root->mNumChildren; i < in; ++i)
  {
    aiNode *child = root->mChildren[i];
    CollectMeshes(meshes, scene, child, parentMatrix);
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