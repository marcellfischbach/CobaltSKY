#pragma once

#include <QString>
#include <QDir>

struct AssetManagerNewHandler
{
  virtual ~AssetManagerNewHandler() { }

  virtual QString GetTypeName() const = 0;
  virtual bool CreateNewAsset(const QDir &folder, const QString &assetName) = 0;
};