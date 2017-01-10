#pragma once


#include <QAbstractItemModel>


class AssetManagerFolderModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  AssetManagerFolderModel();
  virtual ~AssetManagerFolderModel();
};