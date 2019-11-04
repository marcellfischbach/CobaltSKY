

#include <editorcore/glcontext.hh>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QWidget>
#include <iostream>
#include <Windows.h>

namespace cs::editor::core
{


GLContext::GLContext()
  : m_context(nullptr)
  , m_surface(nullptr)
{

  QSurfaceFormat format;
  format.setRedBufferSize(8);
  format.setGreenBufferSize(8);
  format.setBlueBufferSize(8);
  format.setAlphaBufferSize(8);

  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);

  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setVersion(4, 6);
  format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
  format.setRenderableType(QSurfaceFormat::OpenGL);



  m_surface = new QOffscreenSurface();
  m_surface->setFormat(format);
  m_surface->create();

  m_context = new QOpenGLContext();
  m_context->setFormat(format);
  m_context->setShareContext(QOpenGLContext::globalShareContext());
  m_context->create();
}

bool GLContext::MakeCurrent()
{
  if (!m_context->makeCurrent(m_surface))
  {
    std::cerr << "Unable to make context current\n";
    return false;
  }
  return true;
}

QOpenGLContext* GLContext::GetQOpenGLContext()
{
  return m_context;
}


GLContext& GLContext::Get()
{
  static GLContext static_context;
  return static_context;
}
}