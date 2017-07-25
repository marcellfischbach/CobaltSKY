
#pragma once

#include <importers/modelimporter.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/math/csvector.hh>
#include <vector>
#include <map>


class QString;
class StaticMeshModelImporterData : public ModelImporterData
{
  friend class ModelImporter;
public:
  StaticMeshModelImporterData();
  virtual ~StaticMeshModelImporterData();

  virtual void SetName(const QString &name);

  virtual void GenerateData(const aiScene *scene);

  virtual csResourceLocator Import(AssetManagerWidget *assetManager);



private:
  StaticMeshEditorWidget *m_staticMeshEditorWidget;

  std::vector<aiNode*> m_meshNodes;


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
    SubMesh *subMesh;
    unsigned materialIndex;
  };

  struct LOD
  {
    unsigned lod;
    std::vector<Data*> datas;
  };

  std::vector<LOD*> m_lods;
  std::map<unsigned, unsigned> m_modelToLocalMap;


  LOD *GetLOD(unsigned lod);
  Data *GetData(unsigned lod, unsigned materialIndex);
  Data *GetData(LOD *lod, unsigned materialIndex);

  unsigned GetLocalMaterialIndex(unsigned modelIndex);
};
