
#include <graphicsgl4/gl4rendertarget.hh>
#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4defines.hh>

csRenderTargetGL4::csRenderTargetGL4()
  : iRenderTarget()
  , m_name(0)
  , m_depthTexture(0)
  , m_provided(false)
{
  CS_CLASS_GEN_CONSTR;
}

csRenderTargetGL4::csRenderTargetGL4(GLuint name, csUInt16 width, csUInt16 height)
  : iRenderTarget()
  , m_name(name)
  , m_depthTexture(0)
  , m_width(width)
  , m_height(height)
  , m_provided(true)
{
  CS_CLASS_GEN_CONSTR;
}

csRenderTargetGL4::~csRenderTargetGL4()
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

    for (csTextureGL4 *txt : m_colorTextures)
    {
      CS_RELEASE(txt);
    }
    m_colorTextures.clear();
  }
  CS_CHECK_GL_ERROR;
}

void csRenderTargetGL4::Bind()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_CHECK_GL_ERROR;
}

csUInt16 csRenderTargetGL4::GetWidth() const
{
  return m_width;
}

csUInt16 csRenderTargetGL4::GetHeight() const
{
  return m_height;
}

void csRenderTargetGL4::Setup(GLuint name, csUInt16 width, csUInt16 height)
{
  m_name = name;
  m_width = width;
  m_height = height;
  m_provided = true;
}

void csRenderTargetGL4::Initialize(csUInt16 width, csUInt16 height)
{
  m_width = width;
  m_height = height;
  glGenFramebuffers(1, &m_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
  CS_CHECK_GL_ERROR;
}

void csRenderTargetGL4::AddColorTexture(iTexture *color)
{
  csTextureGL4 *coloGL4 = csQueryClass<csTextureGL4>(color);
  if (coloGL4)
  {
    color->AddRef();
    glFramebufferTexture(GL_FRAMEBUFFER, (GLenum)(GL_COLOR_ATTACHMENT0 + m_colorTextures.size()), coloGL4->GetName(), 0);
    m_colorTextures.push_back(coloGL4);
    CS_CHECK_GL_ERROR;

  }
}

void csRenderTargetGL4::SetDepthTexture(iTexture *depth)
{
  csTextureGL4 *depthGL4 = csQueryClass<csTextureGL4>(depth);
  if (depthGL4 != m_depthTexture)
  {
    CS_SET(m_depthTexture, depthGL4);
    if (depthGL4)
    {
      glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, depthGL4->GetName(), 0);
      CS_CHECK_GL_ERROR;
    }
  }
}


void csRenderTargetGL4::SetDepthBuffer(csUInt16 width, csUInt16 height)
{
  glGenRenderbuffers(1, &m_depthBuffer);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthBuffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_depthBuffer);
  CS_CHECK_GL_ERROR;
}


bool csRenderTargetGL4::Finilize()
{
  GLenum r = glCheckFramebufferStatus(GL_FRAMEBUFFER);

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  CS_CHECK_GL_ERROR;
  return r == GL_FRAMEBUFFER_COMPLETE;
}

iTexture *csRenderTargetGL4::GetColorBuffer(csUInt8 idx) const
{
  if (idx >= m_colorTextures.size())
  {
    return 0;
  }

  return m_colorTextures[idx];
}

iTexture *csRenderTargetGL4::GetDepthBuffer() const
{
  return m_depthTexture;
}

