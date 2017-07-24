#pragma once

#include <assetmanager/import/assetmanagerimporter.hh>
#include <vector>

struct aiNode;
struct aiScene;
class QWidget;
class ModelImporterData : public AssetManagerImportData
{
  friend class ModelImporter;
public:
  ModelImporterData();
  virtual ~ModelImporterData();


  virtual void SetName(const QString &name);
  virtual const QString &GetName() const;
  virtual QWidget *GetWidget() const;

  virtual csResourceLocator Import(AssetManagerWidget *assetManager);

private:
  QString m_name;
  QString m_fileName;
  QWidget *m_view;

  std::vector<aiNode*> m_meshNodes;
  std::vector<aiNode*> m_skeletons;
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
  void Scan(ModelImporterData *data, const aiScene *scene) const;
  void Scan(ModelImporterData *data, const aiScene *scene, aiNode *node, int indent = 0) const;

  bool IsSkeleton(const std::string &name) const;
};