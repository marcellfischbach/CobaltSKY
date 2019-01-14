#include <editorcore/editor.hh>
#include <editorcore/mainwindow/mainwindow.hh>
#include <editorcore/assetmanager/assetmanagerview.hh>

namespace editor
{

Editor::Editor()
  : m_mainWindow(nullptr)
{

}


Editor *Editor::Get()
{
 static Editor editor;
 return &editor;
}

MainWindow *Editor::GetMainWindow()
{
  return m_mainWindow;
}

bool Editor::Initialize(int arg, char **argv)
{
  m_mainWindow = new MainWindow ();


  m_mainWindow->AddView(AssetManagerView::Get());


  return true;
}

}
