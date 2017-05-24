
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
  m_offscreenSurface = new QOffscreenSurface();
  m_offscreenSurface->create();

  m_openglContext = new QOpenGLContext(parent);
  m_openglContext->setFormat(QSurfaceFormat::defaultFormat());
  m_openglContext->setShareContext(QOpenGLContext::globalShareContext());
  m_openglContext->create();
}
