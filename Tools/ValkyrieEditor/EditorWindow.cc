
#include <EditorWindow.hh>
#include <SceneView.hh>
#include <ShaderGraph/ShaderGraphView.hh>
#include <qtimer.h>

EditorWindow::EditorWindow()
  : QMainWindow()
{

  m_mainTabWidget = new QTabWidget();
  setCentralWidget(m_mainTabWidget);

  //m_sceneView = new SceneView();
  //m_mainTabWidget->addTab(m_sceneView, "Scene");

  m_shaderGraphView = new ShaderGraphView(this);
  m_mainTabWidget->addTab(m_shaderGraphView, "ShaderGraph");

  //m_mainTabWidget->setCurrentIndex(1);

  QTimer::singleShot(100, this, SLOT(switchView()));
}


EditorWindow::~EditorWindow()
{

}

void EditorWindow::switchView()
{
  m_mainTabWidget->setCurrentIndex(0);
  m_shaderGraphView->Set(vkResourceLocator("${materials}/my_material.asset"));
}


