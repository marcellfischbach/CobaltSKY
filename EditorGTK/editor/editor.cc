
#include <editor/editor.hh>
#include <editor/mainwindow.hh>

Editor::Editor()
{
  m_mainWindow = new MainWindow();
}


Editor *Editor::Get()
{
  static Editor editor;
  return &editor;
}

