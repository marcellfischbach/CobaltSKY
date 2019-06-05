

#include <editorcore/renderwidget.hh>
#include <editorcore/glcontext.hh>
#include <math.h>
#include <iostream>
#include <QOpenGLContext>

namespace cs::editor::core
{

RenderWidget::RenderWidget(QWidget* parent)
  : QOpenGLWidget(parent)
{
  m_red = (float)rand() / (float)RAND_MAX;
  m_green = (float)rand() / (float)RAND_MAX;
  m_blue = (float)rand() / (float)RAND_MAX;

}


void RenderWidget::initializeGL()
{
  std::cout << "Context: " << context() << std::endl;
  context()->setShareContext(GLContext::Get().GetContext());
  context()->create();
  //context()->makeCurrent(context()->surface());

  //QOpenGLWidget::initializeGL();

}

void RenderWidget::paintGL()
{
  glClearColor(m_red, m_green, m_blue, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  std::cout << "Is Sharing: " << QOpenGLContext::areSharing(context(), GLContext::Get().GetContext()) << std::endl;
}

void RenderWidget::resizeGL(int width, int height)
{

}



}