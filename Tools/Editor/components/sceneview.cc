

#include <components/sceneview.hh>
#include <QPaintEvent>

SceneView::SceneView(QWidget *parent)
  : QWidget(parent)
{

}

SceneView::~SceneView()
{

}


void SceneView::paintEvent(QPaintEvent *event)
{
}


/*
void SceneView::initializeGL()
{
  QOpenGLWidget::initializeGL();
}

void SceneView::paintGL()
{
  QOpenGLWidget::paintGL();
  glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
  glClearDepth(1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void SceneView::resizeGL(int w, int h)
{
  QOpenGLWidget::resizeGL(w, h);
  glViewport(0, 0, w, h);
}
*/
