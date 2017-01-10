
#include <Editor.hh>
#include <MainWindow.hh>
#include <AssetManager/AssetManager.hh>

Editor::Editor()
  : m_mainWindow(0)
{
  VK_CLASS_GEN_CONSTR;
}


bool Editor::Initialize(int argc, char **argv)
{
  m_mainWindow = new MainWindow();

  m_assetManager = new AssetManagerWidget();

  m_mainWindow->addDockWidget(Qt::BottomDockWidgetArea, new AssetManagerDock(m_mainWindow));


  m_mainWindow->showMaximized();
  return true;
}

MainWindow *Editor::GetMainWindow()
{
  return m_mainWindow;
}

AssetManagerWidget *Editor::GetAssetManager()
{
  return m_assetManager;
}

Editor *Editor::Get()
{
  static Editor editor;
  return &editor;
}
