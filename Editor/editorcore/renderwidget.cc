

#include <editorcore/renderwidget.hh>
#include <math.h>
#include <iostream>
#include <QOpenGLContext>

namespace cs::editor::core
{

RenderWidget::RenderWidget(QWidget* parent)
  : QOpenGLWidget(parent)
{
  QSurfaceFormat format;
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);

  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);

  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 4);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  setFormat(format);

  m_red = (float)rand() / (float)RAND_MAX;
  m_green = (float)rand() / (float)RAND_MAX;
  m_blue = (float)rand() / (float)RAND_MAX;

}


void RenderWidget::initializeGL()
{

}

void RenderWidget::paintGL()
{
  glClearColor(0.5f, 0.0f, 0.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
}


void RenderWidget::resizeGL(int width, int height)
{

}



}