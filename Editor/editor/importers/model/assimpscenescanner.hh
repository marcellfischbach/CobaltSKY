
#pragma once

#include <editor/editorexport.hh>
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

  const std::vector<AssimpMeshData> &GetMeshes() const;
  const std::vector<AssimpMeshData> &GetCollision() const;

private:

  void Scan(const aiScene *scene, aiNode *node, const csMatrix4f &matrix);

  std::vector<AssimpMeshData> m_meshData;
  std::vector<AssimpMeshData> m_collisionData;
};



