
#pragma once

#include <importers/modelimporter.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <vector>
#include <map>


class csAssetOutputStream;
class QString;
class StaticMeshModelImporterData : public ModelImporterData
{
public:
  StaticMeshModelImporterData();
  virtual ~StaticMeshModelImporterData();

  virtual void SetImportName(const QString &importName);
  virtual void SetName(const QString &name);

  virtual void GenerateData(const aiScene *scene);

  void AddNode(aiNode *node);

  virtual csResourceLocator Import(AssetManagerWidget *assetManager);

private:
  csResourceLocator ImportMesh(AssetManagerWidget *assetManager);
  csResourceLocator ImportCollisionMesh(AssetManagerWidget *assetManager);

  void GenerateMeshData (const aiScene *scene);
  void GenerateColliderData (const aiScene *scene);

  bool m_importCollider;

  
  StaticMeshEditorWidget *m_staticMeshEditorWidget;

  std::vector<aiNode*> m_meshNodes;
  std::vector<aiNode*> m_collisionNodes;


  struct Vertex
  {
    csVector3f position;
    csVector3f normal;
    csVector3f tangent;
    csVector3f binormal;
    csVector2f texCoord;
  };

  struct SubMesh
  {
    csBoundingBox boundingBox;
    std::vector<Vertex> vertices;
    std::vector<unsigned> trigonIndices;
  };

  struct Data
  {
    std::string name;
    SubMesh *subMesh;
    unsigned materialIndex;
  };

  struct LOD
  {
    unsigned lod;
    std::vector<Data*> datas;
  };

  enum ColliderType
  {
    eCT_Box,
    eCT_Cylinder,
    eCT_TriMesh
  };

  struct Collider
  {
    ColliderType type;
    csVector3f halfExtends;
    csMatrix4f transform;
    std::vector<csVector3f> vertices;
    std::vector<unsigned> trigonIndices;

  };

  std::vector<LOD*> m_lods;
  std::map<unsigned, unsigned> m_modelToLocalMap;

  std::map<unsigned, std::string> m_materialNames;

  std::vector<Collider> m_colliders;


  LOD *GetLOD(unsigned lod);
  Data *GetData(unsigned lod, unsigned materialIndex);
  Data *GetData(LOD *lod, unsigned materialIndex);
  void Output(SubMesh *subMesh, csAssetOutputStream &os);

  unsigned GetLocalMaterialIndex(unsigned modelIndex, const std::string &materialName);
};
