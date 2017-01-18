
#include <Editor.hh>
#include <MainWindow.hh>
#include <assetmanager/assetmanagerdock.hh>
#include <assetmanager/assetmanagerwidget.hh>
#include <valkyrie/core/vkvfs.hh>

#include <QApplication>
#include <QDesktopWidget>

Editor::Editor()
  : m_mainWindow(0)
{
  VK_CLASS_GEN_CONSTR;
}


bool Editor::Initialize(int argc, char **argv)
{
  m_rootPath = QDir(QString(vkVFS::Get()->GetRootPath().c_str()));
  printf("RootPath: %s\n", (const char*)m_rootPath.dirName().toLatin1());

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

QRect Editor::GetScreenSize()
{
  QDesktopWidget *desktop = QApplication::desktop();
  int screen = desktop->screenNumber(m_mainWindow);
  QWidget *widget = desktop->screen(screen);
  return QRect(0, 0, widget->width(), widget->height());
}
