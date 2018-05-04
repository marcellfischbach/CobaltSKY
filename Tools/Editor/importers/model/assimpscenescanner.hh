
#pragma once

#include <string>
#include <cobalt/math/csmatrix.hh>
#include <vector>

struct aiMesh;
struct aiNode;
struct aiScene;

struct AssimpMeshData
{
  csMatrix4f tr;
  aiMesh *mesh;
};

class AssimpSceneScanner
{
public:
  AssimpSceneScanner();

  bool Scan(const std::string &fileName);

private:

  void Scan(const aiScene *scene, aiNode *node, const csMatrix4f &matrix);

  std::vector<AssimpMeshData> m_meshData;
  std::vector<AssimpMeshData> m_collisionData;
};

