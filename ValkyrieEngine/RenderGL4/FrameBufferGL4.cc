
#include <RenderGL4/FrameBufferGL4.hh>
#include <RenderGL4/TextureGL4.hh>

vkFrameBufferGL4::vkFrameBufferGL4()
  : IFrameBuffer ()
  , m_name(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkFrameBufferGL4::~vkFrameBufferGL4()
{
  if (m_name != 0)
  {
    glDeleteFramebuffers(1, &m_name);
  }
  if (m_depthBuffer)
  {
    m_depthBuffer->Release();
    m_depthBuffer = 0;
  }
  for (vkTexture2DGL4 *txt : m_colorBuffers)
  {
    txt->Release();
  }
  m_colorBuffers.clear();
}

void vkFrameBufferGL4::Initialize()
{
  glGenFramebuffers(1, &m_name);
  glBindFramebuffer(GL_FRAMEBUFFER, m_name);
}

void vkFrameBufferGL4::AddColorTexture(vkTexture2DGL4 *color)
{
  if (color)
  {
    color->AddRef();
    m_colorBuffers.push_back(color);

  }
}

void vkFrameBufferGL4::SetDepthTexture(vkTexture2DGL4 *depth)
{
  VK_SET(m_depthBuffer, depth);
}

