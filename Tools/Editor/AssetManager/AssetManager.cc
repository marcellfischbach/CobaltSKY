
#include <AssetManager/AssetManager.hh>
#include <Editor.hh>

AssetManagerWidget::AssetManagerWidget()
  : QWidget()
{
  m_gui.setupUi(this);
}

AssetManagerWidget::~AssetManagerWidget()
{

}


AssetManagerDock::AssetManagerDock(QWidget *parent)
  : QDockWidget (tr ("AssetManager"), parent)
{
  setWidget(Editor::Get()->GetAssetManager());
  setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
}


AssetManagerDock::~AssetManagerDock()
{

}
