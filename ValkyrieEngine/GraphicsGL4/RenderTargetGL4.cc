
#include <GraphicsGL4/RenderTargetGL4.hh>
#include <GraphicsGL4/TextureGL4.hh>
#include <GraphicsGL4/DefinesGL4.hh>

vkRenderTargetGL4::vkRenderTargetGL4()
  : IRenderTarget()
  , m_name(0)
  , m_depthTexture(0)
  , m_provided(false)
{
  VK_CLASS_GEN_CONSTR;
}

vkRenderTargetGL4::vkRenderTargetGL4(GLuint name, vkUInt16 width, vkUInt16 height)
  : IRenderTarget()
  , m_name(name)
  , m_depthTexture(0)
  , m_width(width)
  , m_height(height)
  , m_provided(true)
{
  VK_CLASS_GEN_CONSTR;
}

vkRenderTargetGL4::~vkRenderTargetGL4()
{
  if (!m_provided)
  {
    if (m_name != 0)
    {
      glDeleteFramebuffers(1, &m_name);
    }

    if (m_depthBuffer)
    {
      glDeleteRenderbuffers(1, &m_depthBuffer);
      m_depthBuffer = 0;
    }

    VK_RELEASE(m_depthTexture);
    m_depthTexture = 0;

    for (vkTextureGL4 *txt : m_colorTextures)
    {
      VK_RELEASE(txt);
    }
    m_colorTextures.clear();
  }
}

void vkRenderTargetGL4::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

vkUInt16 vkRenderTargetGL4::GetWidth() const
{
  return m_width;
}

vkUInt16 vkRenderTargetGL4::GetHeight() const
{
  return m_height;
}

void vkRenderTargetGL4::Setup(GLuint name, vkUInt16 width, vkUInt16 height)
{
  m_name = name;
  m_width = width;
  m_height = height;
}

void vkRenderTargetGL4::Initialize(vkUInt16 width, vkUInt16 height)
{
  m_width = width;
  m_height = height;
  glGenFramebuffers(1, &m_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

void vkRenderTargetGL4::AddColorTexture(ITexture *color)
{
  VK_CHECK_GL_ERROR;
  vkTextureGL4 *coloGL4 = vkQueryClass<vkTextureGL4>(color);
  if (coloGL4)
  {
    color->AddRef();
    glFramebufferTexture(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + m_colorTextures.size()), coloGL4->GetName(), 0);
    m_colorTextures.push_back(coloGL4);

  }

  VK_CHECK_GL_ERROR;
}

void vkRenderTargetGL4::SetDepthTexture(ITexture *depth)
{
  vkTextureGL4 *depthGL4 = vkQueryClass<vkTextureGL4>(depth);
  if (depthGL4 != m_depthTexture)
  {
    VK_SET(m_depthTexture, depthGL4);
    if (depthGL4)
    {
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthGL4->GetName(), 0);
      VK_CHECK_GL_ERROR;
    }
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
  VK_CHECK_GL_ERROR;
  GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);
  VK_CHECK_GL_ERROR;

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  VK_CHECK_GL_ERROR;
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

