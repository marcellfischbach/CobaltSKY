

#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4defines.hh>



vkTextureGL4::vkTextureGL4(vkTextureType type)
  : ITexture()
  , m_name(0)
  , m_type(type)
  , m_target(textureTypeMap[type])
  , m_sampler(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkTextureGL4::~vkTextureGL4()
{
  if (m_name)
  {
    glDeleteTextures(1, &m_name);
    VK_CHECK_GL_ERROR;
    m_name = 0;
  }
  VK_RELEASE(m_sampler);
  m_sampler = 0;
}

bool vkTextureGL4::Initialize()
{
  glGenTextures(1, &m_name);
  VK_CHECK_GL_ERROR;
  return m_name != 0;
}

void vkTextureGL4::SetSampler(ISampler *sampler)
{
  VK_SET(m_sampler, sampler);
}

ISampler *vkTextureGL4::GetSampler()
{
  return m_sampler;
}

const ISampler *vkTextureGL4::GetSampler() const
{
  return m_sampler;
}

void vkTextureGL4::Bind()
{
  glBindTexture(m_target, m_name);
  VK_CHECK_GL_ERROR;
}

vkTextureType vkTextureGL4::GetType() const
{
  return m_type;
}

void vkTextureGL4::GenerateMipMaps()
{
  Bind();
  glGenerateMipmap(m_target);
}




