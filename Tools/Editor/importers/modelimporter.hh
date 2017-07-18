#pragma once

#include <assetmanager/import/assetmanagerimporter.hh>

class QWidget;
class ModelImporterData : public AssetManagerImportData
{
public:
  ModelImporterData();
  virtual ~ModelImporterData();

  void Import(const QString &fileName);


  virtual const QString &GetName() const;
  virtual QWidget *GetWidget() const;

  virtual csResourceLocator Import(AssetManagerWidget *assetManager);

private:
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
  virtual AssetManagerImportData *Import(const QString &fileName) const;

};