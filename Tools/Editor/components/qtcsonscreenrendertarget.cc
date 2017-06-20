

#include <components/qtcsonscreenrendertarget.hh>
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
    m_renderTarget = new csRenderTargetGL4(name, width, height);
  }
  else
  {
    m_renderTarget->Setup(name, width, height);
  }

}


iRenderTarget *QTCSOnscreenRenderTarget::GetRenderTarget()
{
  return m_renderTarget;
}
