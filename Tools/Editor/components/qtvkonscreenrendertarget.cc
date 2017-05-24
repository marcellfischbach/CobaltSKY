

#include <components/qtvkonscreenrendertarget.hh>
#include <graphicsgl4/gl4rendertarget.hh>

QTVKOnscreenRenderTarget::QTVKOnscreenRenderTarget()
{

}

QTVKOnscreenRenderTarget::~QTVKOnscreenRenderTarget()
{
  VK_RELEASE(m_renderTarget);
}

void QTVKOnscreenRenderTarget::Setup(unsigned width, unsigned height)
{
  GLint name;
  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &name);

  if (!m_renderTarget)
  {
    m_renderTarget = new vkRenderTargetGL4(name, width, height);
  }
  else
  {
    m_renderTarget->Setup(name, width, height);
  }

}


iRenderTarget *QTVKOnscreenRenderTarget::GetRenderTarget()
{
  return m_renderTarget;
}
