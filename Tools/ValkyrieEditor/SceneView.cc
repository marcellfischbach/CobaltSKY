

#include <SceneView.hh>
#include <qopenglcontext.h>


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
  printf("SceneView::initializeGL(): %p %p\n", context(), context()->shareContext());
}

void SceneView::paintGL()
{
  glClearColor(0.0f, 0.0f, 0.1f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}

void SceneView::resizeGL(int w, int h)
{

}