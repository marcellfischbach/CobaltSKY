
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

  m_offscreenSurface = new QOffscreenSurface();
  m_offscreenSurface->setFormat(format);
  m_offscreenSurface->create();

  m_openglContext = QOpenGLContext::globalShareContext();
  m_openglContext->create();
  m_openglContext->makeCurrent(m_offscreenSurface);
  

}


bool GLContext::MakeCurrent()
{
  if (m_openglContext && m_offscreenSurface)
  {
    return m_openglContext->makeCurrent(m_offscreenSurface);
  }
  return false;
}

bool GLContext::Unbind()
{
  if (m_openglContext)
  {
    //return m_openglContext->makeCurrent(0);
  }
  return false;
}