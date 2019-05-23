
#include <graphicsgl4/gl4rendertarget.hh>
#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4defines.hh>

cs::RenderTargetGL4::RenderTargetGL4()
  : cs::iRenderTarget()
  , m_name(0)
  , m_depthTexture(0)
  , m_provided(false)
{
  CS_CLASS_GEN_CONSTR;
}

cs::RenderTargetGL4::RenderTargetGL4(GLuint name, csUInt16 width, csUInt16 height)
  : cs::iRenderTarget()
  , m_name(name)
  , m_depthTexture(0)
  , m_width(width)
  , m_height(height)
  , m_provided(true)
{
  CS_CLASS_GEN_CONSTR;
}

cs::RenderTargetGL4::~RenderTargetGL4()
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

    CS_RELEASE(m_depthTexture);
    m_depthTexture = 0;

    for (cs::TextureWrapper *txt : m_colorTextures)
    {
      CS_RELEASE(txt);
    }
    m_colorTextures.clear();
  }
  CS_CHECK_GL_ERROR;
}

void cs::RenderTargetGL4::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_CHECK_GL_ERROR;
}

csUInt16 cs::RenderTargetGL4::GetWidth() const
{
  return m_width;
}

csUInt16 cs::RenderTargetGL4::GetHeight() const
{
  return m_height;
}

void cs::RenderTargetGL4::Setup(GLuint name, csUInt16 width, csUInt16 height)
{
  m_name = name;
  m_width = width;
  m_height = height;
  m_provided = true;
}

void cs::RenderTargetGL4::Initialize(csUInt16 width, csUInt16 height)
{
  m_width = width;
  m_height = height;
  glGenFramebuffers(1, &m_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_CHECK_GL_ERROR;
}

void cs::RenderTargetGL4::AddColorTexture(cs::TextureWrapper *color)
{
  cs::TextureGL4 *coloGL4 = cs::QueryClass<cs::TextureGL4>(color->Get());
  if (coloGL4)
  {
    color->AddRef();
    glFramebufferTexture(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + m_colorTextures.size()), coloGL4->GetName(), 0);
    m_colorTextures.push_back(color);
    CS_CHECK_GL_ERROR;

  }
}

void cs::RenderTargetGL4::SetDepthTexture(cs::TextureWrapper *depth)
{
  if (depth != m_depthTexture)
  {
    cs::TextureGL4 *depthGL4 = cs::QueryClass<cs::TextureGL4>(depth->Get());
    CS_SET(m_depthTexture, depth);
    if (depthGL4)
    {
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthGL4->GetName(), 0);
      CS_CHECK_GL_ERROR;
    }
  }
}


void cs::RenderTargetGL4::SetDepthBuffer(csUInt16 width, csUInt16 height)
{
  glGenRenderbuffers(1, &m_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
  CS_CHECK_GL_ERROR;
}


bool cs::RenderTargetGL4::Finilize()
{
  GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CS_CHECK_GL_ERROR;
  return r == GL_FRAMEBUFFER_COMPLETE;
}

cs::TextureWrapper *cs::RenderTargetGL4::GetColorBuffer(csUInt8 idx) const
{
  if (idx >= m_colorTextures.size())
  {
    return 0;
  }

  return m_colorTextures[idx];
}

cs::TextureWrapper *cs::RenderTargetGL4::GetDepthBuffer() const
{
  return m_depthTexture;
}

