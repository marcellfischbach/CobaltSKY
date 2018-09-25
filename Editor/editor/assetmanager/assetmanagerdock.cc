
#include <editor/assetmanager/assetmanagerdock.hh>
#include <editor/AssetManager/assetmanagerwidget.hh>
#include <editor/editor.hh>

AssetManagerDock::AssetManagerDock(QWidget *parent)
  : QDockWidget(tr("AssetManager"), parent)
{
  setWidget(Editor::Get()->GetAssetManager());
  setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
}


AssetManagerDock::~AssetManagerDock()
{

}
