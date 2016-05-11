

#include <SceneView.hh>
#include <qopenglcontext.h>
#include <Editor.hh>
#include <Valkyrie/Engine.hh>


SceneView::SceneView()
  : QOpenGLWidget()
{

}

SceneView::~SceneView()
{

}

void SceneView::initializeGL()
{
  context()->setShareContext(QOpenGLContext::globalShareContext());


  Editor::Get().RequestGraphics();


  IGraphics *graphics = vkEngine::Get()->GetRenderer();
  graphics->ResetDefaults();

}

void SceneView::paintGL()
{
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void SceneView::resizeGL(int w, int h)
{

}