#pragma once

#include <QString>
#include <QStringList>
#include <QWidget>
#include <cobalt/core/csresourcelocator.hh>

class AssetManagerWidget;
struct AssetManagerImportData
{
  AssetManagerImportData() {}
  virtual ~AssetManagerImportData() { }

  virtual const QString &GetName() const = 0;
  virtual QWidget *GetWidget() const = 0;

  virtual void Import(AssetManagerWidget *assetManager) = 0;

};

struct AssetManagerImporter
{
  AssetManagerImporter() { }
  virtual ~AssetManagerImporter() { }


  virtual const QStringList GetFilters() const = 0;

  virtual bool CanImport(const QString &fileName) const = 0;
  virtual AssetManagerImportData *Import(const QString &fileName) const = 0;

};