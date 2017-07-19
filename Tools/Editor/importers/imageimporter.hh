#pragma once

#include <assetmanager/import/assetmanagerimporter.hh>
#include <QGraphicsView>

class AssetManagerWidget;
class ImageImporterData : public AssetManagerImportData
{
public:
  ImageImporterData();
  virtual ~ImageImporterData();

  void Import(const QString &fileName);

  virtual const QString &GetName() const;
  virtual QWidget *GetWidget() const;

  virtual csResourceLocator Import(AssetManagerWidget *assetManager);

private:
  QString GetType() const;
  QGraphicsView *m_view;
  QString m_name;
  QString m_fileName;
};


class ImageImporter : public AssetManagerImporter
{
public:
  ImageImporter();
  virtual ~ImageImporter();

  virtual const QStringList GetFilters() const;

  virtual bool CanImport(const QString &fileName) const;
  virtual AssetManagerImportData *Import(const QString &fileName) const;

};