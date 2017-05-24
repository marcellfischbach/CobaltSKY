
#include <valkyrie/vkengine.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>

#include <GL/glew.h>

#include <QOpenGLContext>
#include <renderwidget.hh>
#include <editor.hh>

RenderWidget::RenderWidget(QWidget *parent, Qt::WindowFlags f)
  : QOpenGLWidget(parent, f)
  , m_name("Unnamed")
  , m_renderTarget(0)
  , m_clear(false)
{
  setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

RenderWidget::~RenderWidget()
{

}

void RenderWidget::initializeGL()
{
  printf("RenderWidget::initializeGL\n");
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
  iGraphics *gr = vkEng->GetRenderer();
  
  gr->SetRenderTarget(m_renderTarget);
  gr->SetViewport(m_renderTarget);
  gr->ResetDefaults();

  if (m_clear)
  {
    gr->Clear(true, vkVector4f (0.0f, 0.0f, 0.5f, 1.0f));
  }
}



void RenderWidget::resizeGL(int width, int height)
{

}
