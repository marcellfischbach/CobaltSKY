
#include <Editor.hh>
#include <EditorWindow.hh>

Editor::Editor()
  : QObject()
{

}

bool Editor::Initialize(int argc, char **argv)
{
  Q_UNUSED(argc);
  Q_UNUSED(argv);

  m_editorWindow = new EditorWindow();
  m_editorWindow->resize(1024, 768);
  m_editorWindow->setVisible(true);

  return true;
}


Editor &Editor::Get()
{
  static Editor editor;
  return editor;
}