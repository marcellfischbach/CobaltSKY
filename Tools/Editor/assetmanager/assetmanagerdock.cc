
#include <assetmanager/assetmanagerdock.hh>
#include <Editor.hh>

AssetManagerDock::AssetManagerDock(QWidget *parent)
  : QDockWidget(tr("AssetManager"), parent)
{
  setWidget(Editor::Get()->GetAssetManager());
  setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
}


AssetManagerDock::~AssetManagerDock()
{

}
