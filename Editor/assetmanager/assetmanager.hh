
#pragma once

#include <assetmanager/assetmanagerexport.hh>

#include <QWidget>

namespace cs::editor::model
{
class Model;
}

namespace cs::editor::assetmanager
{


class ASSET_MANAGER_API AssetManager : public QWidget
{
public:
  AssetManager(model::Model *model, QWidget *parent = nullptr);
  virtual ~AssetManager() { }

};

}