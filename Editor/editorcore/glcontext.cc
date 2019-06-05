

#include <editorcore/glcontext.hh>
#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <iostream>

namespace cs::editor::core
{


GLContext::GLContext()
  : m_context(nullptr)
  , m_surface(nullptr)
{
  m_surface = new QOffscreenSurface();
  m_surface->create();

  m_context = new QOpenGLContext();
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
  m_context->setFormat(format);
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


GLContext& GLContext::Get()
{
  static GLContext static_context;
  return static_context;
}
}