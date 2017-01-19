
#include <editor.hh>
#include <editormodule.hh>
#include <iasseteditor.hh>
#include <mainwindow.hh>
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
  EditorModule::Initialize();

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

void Editor::AddEditorFactory(iAssetEditorFactory *factory)
{
  m_editorFactories.push_back(factory);
}

void Editor::OpenAsset(const AssetDescriptor &desc)
{
  iAssetEditor* editor = 0;
  if (m_openEditors.find(desc) != m_openEditors.end())
  {
    editor = m_openEditors[desc];
  }
  else
  {
    iAssetEditorFactory *factory = FindFactory(desc);
    if (!factory)
    {
      return;
    }

    editor = factory->CreateEditor(desc);
    if (!editor)
    {
      return;
    }
    editor->SetAssetDescriptor(desc);
  }


  if (m_mainWindow->ShowEditor(editor))
  {
    m_openEditors[desc] = editor;
  }


}

iAssetEditorFactory *Editor::FindFactory(const AssetDescriptor &desc)
{
  for (iAssetEditorFactory *factory : m_editorFactories)
  {
    if (factory->CanEdit(desc))
    {
      return factory;
    }
  }
  return 0;
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
