#pragma once

#include <editor/editorexport.hh>
#include <editor/assetmanager/import/assetmanagerimporter.hh>
#include <vector>

struct aiNode;
struct aiScene;
class QWidget;
class StaticMeshEditorWidget;
class ModelImporterData : public AssetManagerImportData
{
  friend class newModelImporter;
public:
  enum Type {
    Mesh,
    Collision,
    Skeleton,
    Animation
  };
  
  virtual ~ModelImporterData();

  Type GetType() const;
  virtual void SetImportName(const QString &importName) = 0;
  virtual const QString &GetName() const;
  virtual QWidget *GetWidget() const;

  virtual void GenerateData(const aiScene *scene) = 0;

protected:
  ModelImporterData(Type type);
  void SetView(QWidget *view);

public:
  Type m_type;
  QString m_name;
  QString m_fileName;
  QWidget *m_view;
};

class ModelImporter : public AssetManagerImporter
{
public:
  ModelImporter();
  virtual ~ModelImporter();

  virtual const QStringList GetFilters() const;

  virtual bool CanImport(const QString &fileName) const;
  virtual const std::vector<AssetManagerImportData*> Import(const QString &fileName) const;

private:
  struct Data
  {
    ModelImporterData *GetData(ModelImporterData::Type type);

    std::vector<ModelImporterData*> datas;
  };

  void Scan(Data *data, const aiScene *scene) const;
  void Scan(Data *data, const aiScene *scene, aiNode *node, int indent = 0) const;

  bool IsSkeleton(const std::string &name) const;

};
