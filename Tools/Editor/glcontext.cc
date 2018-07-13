
#include <glcontext.hh>
#include <QOpenGLContext>
#include <QOffscreenSurface>

GLContext::GLContext()
{

}


GLContext *GLContext::Get()
{
    static GLContext ctx;
    return &ctx;
}

void GLContext::Initialize(QObject *parent)
{
  /*
  QSurfaceFormat format;
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 4);
  format.setRenderableType(QSurfaceFormat::OpenGL);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  */

  m_offscreenSurface = new QOffscreenSurface();
  m_offscreenSurface->create();

  m_openglContext = new QOpenGLContext();
  m_openglContext->setShareContext(QOpenGLContext::globalShareContext());
  m_openglContext->create();
  m_openglContext->makeCurrent(m_offscreenSurface);
  
  /*
  m_openglContext = new QOpenGLContext(parent);
  m_openglContext->setFormat(QSurfaceFormat::defaultFormat());
  m_openglContext->setShareContext(QOpenGLContext::globalShareContext());
  m_openglContext->create();
  */
}


bool GLContext::MakeCurrent()
{
  if (m_openglContext && m_offscreenSurface)
  {
    return m_openglContext->makeCurrent(m_offscreenSurface);
  }
  return false;
}