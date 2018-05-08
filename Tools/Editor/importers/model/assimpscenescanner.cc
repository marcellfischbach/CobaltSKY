#include <importers/model/assimpscenescanner.hh>
#include <importers/model/modelmeshexporter.hh>
#include <cobalt/math/csmatrix.hh>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>


AssimpSceneScanner::AssimpSceneScanner()
{

}

void AI2CSMatrix(const aiMatrix4x4 &aiMat, csMatrix4f &csMat)
{
  csMat.m00 = aiMat.a1;
  csMat.m01 = aiMat.b1;
  csMat.m02 = aiMat.c1;
  csMat.m03 = aiMat.d1;

  csMat.m10 = aiMat.a2;
  csMat.m11 = aiMat.b2;
  csMat.m12 = aiMat.c2;
  csMat.m13 = aiMat.d2;

  csMat.m20 = aiMat.a3;
  csMat.m21 = aiMat.b3;
  csMat.m22 = aiMat.c3;
  csMat.m23 = aiMat.d3;

  csMat.m30 = aiMat.a4;
  csMat.m31 = aiMat.b4;
  csMat.m32 = aiMat.c4;
  csMat.m33 = aiMat.d4;
}



void debug_scene (const aiNode *node, int indent)
{
  for (int i=0; i<indent; ++i)
  {
    std::cout << "  ";
  }

  aiMatrix4x4 tr = node->mTransformation;
  std::cout << std::string(node->mName.C_Str())
            << "(" << tr.a4 << ", " << tr.b4 << ", " << tr.c4 << ", " << tr.d4 << ")"
            << std::endl;



  for (unsigned n = 0; n<node->mNumChildren; ++n)
  {
    const aiNode *child = node->mChildren[n];
    debug_scene(child, indent+1);
  }
}

bool AssimpSceneScanner::Scan(const std::string &fileName)
{
  Assimp::Importer importer;
  // And have it read the given file with some example postprocessing
  // Usually - if speed is not the most important aspect for you - you'll
  // propably to request more postprocessing than we do in this example.
  const aiScene* scene = importer.ReadFile(
        fileName,
        aiProcess_CalcTangentSpace |
        aiProcess_FlipWindingOrder |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_SortByPType);

  if (!scene)
  {
    return false;
  }

  csMatrix4f rootMatrix;
  rootMatrix.SetIdentity();
  Scan(scene, scene->mRootNode, rootMatrix);
  return true;
}

void AssimpSceneScanner::Scan(const aiScene *scene, aiNode *node, const csMatrix4f &matrix)
{
  csMatrix4f localMatrix;
  AI2CSMatrix(node->mTransformation, localMatrix);
  csMatrix4f nodeMatrix = csMatrix4f::Mult(matrix, localMatrix, nodeMatrix);

  std::string name = std::string(node->mName.C_Str());
  bool isCollision = name.length() >= 6 && name.substr(0, 6) == std::string("CS_COL");
  for (unsigned i=0; i<node->mNumMeshes; ++i)
  {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    if (mesh)
    {
      AssimpMeshData data;
      data.tr = nodeMatrix;
      data.mesh = mesh;
      if (isCollision)
      {
        m_collisionData.push_back(data);
      }
      else
      {
        m_meshData.push_back(data);
      }
    }
  }


  for (unsigned i=0; i<node->mNumChildren; ++i)
  {
    Scan(scene, node->mChildren[i], nodeMatrix);
  }
}





const std::vector<AssimpMeshData> &AssimpSceneScanner::GetMeshes() const
{
  return m_meshData;
}

const std::vector<AssimpMeshData> &AssimpSceneScanner::GetCollision() const
{
  return m_collisionData;
}
