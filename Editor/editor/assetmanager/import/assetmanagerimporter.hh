#pragma once

#include <editor/editorexport.hh>
#include <QString>
#include <QStringList>
#include <QWidget>
#include <cobalt/core/csresourcelocator.hh>

class AssetManagerWidget;
struct AssetManagerImportData
{
  AssetManagerImportData() {}
  virtual ~AssetManagerImportData() { }

  virtual void SetName(const QString &name) = 0;
  virtual const QString &GetName() const = 0;
  virtual QWidget *GetWidget() const = 0;

  virtual csResourceLocator Import(AssetManagerWidget *assetManager) = 0;

};

struct AssetManagerImporter
{
  AssetManagerImporter() { }
  virtual ~AssetManagerImporter() { }


  virtual const QStringList GetFilters() const = 0;

  virtual bool CanImport(const QString &fileName) const = 0;
  virtual const std::vector<AssetManagerImportData*> Import(const QString &fileName) const = 0;

};