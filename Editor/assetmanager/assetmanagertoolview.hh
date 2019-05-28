

#pragma once

#include <editorcore/itoolview.hh>
#include <assetmanager/assetmanagerexport.hh>

namespace cs::editor::assetmanager
{

class AssetManager;

class ASSET_MANAGER_API AssetManagerToolView : public cs::editor::core::iToolView
{
public:
  AssetManagerToolView();

  virtual Qt::DockWidgetAreas GetAllowedAreas() const;

  virtual Qt::DockWidgetArea GetDefaultArea() const;

  virtual QString GetTitle() const;

  virtual QWidget* GetWidget() const;

private:
  AssetManager* m_assetManager;
};

}