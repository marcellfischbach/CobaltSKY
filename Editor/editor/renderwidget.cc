#include <cobalt/csengine.hh>
#include <cobalt/graphics/igraphics.hh>
#include <cobalt/graphics/itexture2d.hh>
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>

#include <GL/glew.h>

#include <editor/glcontext.hh>
//#include <QOpenGLWidget>
//#include <QOpenGLContext>
#include <editor/renderwidget.hh>
#include <QPainter>
//#include <editor.hh>

RenderWidget::RenderWidget(QWidget *parent, Qt::WindowFlags f)
  : QWidget(parent, f)
  , m_name("Unnamed")
  , m_renderTarget(0)
  , m_clear(false)
  , m_buffer(0)
  , m_bufferImage(0)
{
  //setUpdateBehavior(QOpenGLWidget::NoPartialUpdate);
}

RenderWidget::~RenderWidget()
{

}

void	RenderWidget::paintEvent(QPaintEvent *event)
{

  QPainter p(this);
  
  // GLContext::Get()->MakeCurrent();
  if (!CheckRenderConditions())
  {
    return;
  }

  iGraphics *gr = csEng->GetRenderer();

  gr->SetRenderTarget(m_renderTarget);
  gr->SetViewport(m_renderTarget);
  gr->ResetDefaults();

  if (m_clear)
  {
    gr->Clear(true, csVector4f(0.0f, 0.0f, 0.5f, 1.0f));
  }

  paintGL();

  unsigned size = width() * height() * 4;
  unsigned resSize;
  if (!m_colorTexture->ReadData(0, ePF_R8G8B8A8U, size, m_buffer, resSize))
  {
    return;
  }
  memcpy(m_bufferImage->bits(), m_buffer, resSize);
  QTransform currentTr = p.transform();
  p.setTransform(QTransform::fromScale(1.0f, -1.0f), true);
  p.drawImage(0, -height(), *m_bufferImage);
  p.setTransform(currentTr, false);
  
}

bool RenderWidget::CheckRenderConditions()
{
  /*
  if (!GLContext::Get()->MakeCurrent())
  {
    return false;
  }
  */

  bool fireInitialized = false;
  bool fireResized = false;
  if (m_renderTarget)
  {
    if (m_renderTarget->GetWidth() == width() && m_renderTarget->GetHeight() == height())
    {
      // nothing has changed -> all ok
      return true;
    }
    else
    {
      fireResized = true;
      m_renderTarget->Release();
      m_renderTarget = 0;
      m_colorTexture->Release();
      m_colorTexture = 0;
    }
  }
  else
  {
    fireInitialized = true;
    fireResized = true;
  }


  if (!m_renderTarget)
  {
    // buffer is already initialized but has the wrong size
    m_colorTexture = csEng->GetRenderer()->CreateTexture2D(ePF_R8G8B8A8U, width(), height(), false);
    m_renderTarget = csEng->GetRenderer()->CreateRenderTarget();
    m_renderTarget->Initialize(width(), height());
    m_renderTarget->AddColorTexture(m_colorTexture);
    m_renderTarget->SetDepthBuffer(width(), height());
    if (!m_renderTarget->Finilize())
    {
      printf("Unable to initialize render target\n");
      ReleaseBuffers();
      return false;
    }
  }

  if (fireInitialized)
  {
    initializeGL();
  }
  if (fireResized)
  {
    ReleaseBuffers();

    resizeGL(width(), height());
    m_buffer = new csUInt8[width() * height() * 4];
    m_bufferImage = new QImage(width(), height(), QImage::Format_RGBA8888);

  }
  return true;
}

void RenderWidget::ReleaseBuffers()
{
  if (m_buffer)
  {
    delete[] m_buffer;
    m_buffer = 0;
  }
  if (m_bufferImage)
  {
    delete m_bufferImage;
    m_bufferImage = 0;
  }
}

void RenderWidget::initializeGL()
{
}

void RenderWidget::paintGL()
{
}



void RenderWidget::resizeGL(int width, int height)
{
}

