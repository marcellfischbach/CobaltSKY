#pragma once

#include <assetmanager/import/assetmanagerimporter.hh>
#include <QGraphicsView>

class AssetManagerWidget;
class TextureEditorImportData : public AssetManagerImportData
{
public:
  TextureEditorImportData();
  virtual ~TextureEditorImportData();

  void Import(const QString &fileName);

  virtual const QString &GetName() const;
  virtual QWidget *GetWidget() const;

  virtual void Import(AssetManagerWidget *assetManager);

private:
  QString GetType() const;
  QGraphicsView *m_view;
  QString m_name;
  QString m_fileName;
};


class TextureEditorImporter : public AssetManagerImporter
{
public:
  TextureEditorImporter();
  virtual ~TextureEditorImporter();

  virtual const QStringList GetFilters() const;

  virtual bool CanImport(const QString &fileName) const;
  virtual AssetManagerImportData *Import(const QString &fileName) const;

};