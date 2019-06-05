

#include <editorcore/glcontext.hh>
#include <QOpenGLContext>
#include <iostream>


namespace cs::editor::core
{

GLContext& GLContext::Get()
{
  static GLContext context;
  return context;
}

GLContext::GLContext()
{
  m_context = new QOpenGLContext();
  
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

  m_context->setFormat(format);
  if (!m_context->create())
  {
    std::cerr << "Unable to create OpenGL context.\n";
    m_context->deleteLater();
    m_context = nullptr;
  }

}

bool GLContext::IsValid() const
{
  return m_context != nullptr;
}

QOpenGLContext* GLContext::GetContext() const
{
  return m_context;
}


}