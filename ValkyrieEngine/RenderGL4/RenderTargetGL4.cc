
#include <RenderGL4/RenderTargetGL4.hh>
#include <RenderGL4/TextureGL4.hh>

vkRenderTargetGL4::vkRenderTargetGL4()
  : IRenderTarget ()
  , m_name(0)
  , m_depthTexture(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkRenderTargetGL4::~vkRenderTargetGL4()
{
  if (m_name != 0)
  {
    glDeleteFramebuffers(1, &m_name);
  }
  if (m_depthTexture)
  {
    m_depthTexture->Release();
    m_depthTexture = 0;
  }
  for (vkTextureGL4 *txt : m_colorTextures)
  {
    txt->Release();
  }
  m_colorTextures.clear();
}

void vkRenderTargetGL4::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

void vkRenderTargetGL4::Initialize()
{
  glGenFramebuffers(1, &m_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

void vkRenderTargetGL4::AddColorTexture(ITexture *color)
{
  vkTextureGL4 *coloGL4 = vkQueryClass<vkTextureGL4>(color);
  if (coloGL4)
  {
    color->AddRef();
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_colorTextures.size(), coloGL4->GetName(), 0);
    m_colorTextures.push_back(coloGL4);

  }
}

void vkRenderTargetGL4::SetDepthTexture(ITexture *depth)
{
  vkTextureGL4 *depthGL4 = vkQueryClass<vkTextureGL4>(depth);
  VK_SET(m_depthTexture, depthGL4);
  if (depthGL4)
  {
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthGL4->GetName(), 0);
  }
}


void vkRenderTargetGL4::SetDepthBuffer(vkUInt16 width, vkUInt16 height)
{
  glGenRenderbuffers(1, &m_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
}


bool vkRenderTargetGL4::Finilize()
{
  GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  return r == GL_FRAMEBUFFER_COMPLETE;
}

ITexture *vkRenderTargetGL4::GetColorBuffer(vkUInt8 idx) const
{
  if (idx >= m_colorTextures.size())
  {
    return 0;
  }

  return m_colorTextures[idx];
}

ITexture *vkRenderTargetGL4::GetDepthBuffer() const
{
  return m_depthTexture;
}

