
#include <EditorWindow.hh>
#include <SceneView.hh>
#include <ShaderGraph/ShaderGraphView.hh>

EditorWindow::EditorWindow()
  : QMainWindow()
{

  m_mainTabWidget = new QTabWidget();
  setCentralWidget(m_mainTabWidget);

  m_sceneView = new SceneView();
  m_mainTabWidget->addTab(m_sceneView, "Scene");

  m_shaderGraphView = new ShaderGraphView(this);
  m_mainTabWidget->addTab(m_shaderGraphView, "ShaderGraph");
}


EditorWindow::~EditorWindow()
{

}


