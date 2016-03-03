
#include <EditorWindow.hh>
#include <ShaderGraph/ShaderGraphView.hh>

EditorWindow::EditorWindow()
  : QMainWindow()
{
  m_shaderGraphView = new ShaderGraphView(this);
  setCentralWidget(m_shaderGraphView);
}


EditorWindow::~EditorWindow()
{

}
