
#include <valkyrie/vkengine.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>

#include <GL/glew.h>

#include <renderwidget.hh>
#include <QOpenGLContext>
#include <editor.hh>

RenderWidget::RenderWidget(QWidget *parent, Qt::WindowFlags f)
  : QOpenGLWidget(parent, f)
  , m_name("Unnamed")
  , m_renderTarget(0)
{

}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::initializeGL()
{
  Editor::Get()->GetGraphics()->ResetDefaults();

}

void RenderWidget::paintGL()
{
  QOpenGLWidget::paintGL();

  if (!m_renderTarget)
  {
    m_renderTarget = new vkRenderTargetGL4(defaultFramebufferObject(), width(), height());
  }
  else
  {
    m_renderTarget->Setup(defaultFramebufferObject(), width(), height());
  }
  vkEng->GetRenderer()->SetRenderTarget(m_renderTarget);
  vkEng->GetRenderer()->SetViewport(m_renderTarget);
}



void RenderWidget::resizeGL(int width, int height)
{

}