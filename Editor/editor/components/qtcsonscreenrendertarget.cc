

#include <editor/components/qtcsonscreenrendertarget.hh>
#include <graphicsgl4/gl4rendertarget.hh>

QTCSOnscreenRenderTarget::QTCSOnscreenRenderTarget()
{

}

QTCSOnscreenRenderTarget::~QTCSOnscreenRenderTarget()
{
  CS_RELEASE(m_renderTarget);
}

void QTCSOnscreenRenderTarget::Setup(unsigned width, unsigned height)
{
  GLint name;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &name);

  if (!m_renderTarget)
  {
    m_renderTarget = new cs::RenderTargetGL4(name, width, height);
  }
  else
  {
    m_renderTarget->Setup(name, width, height);
  }

}


cs::iRenderTarget *QTCSOnscreenRenderTarget::GetRenderTarget()
{
  return m_renderTarget;
}
