

#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4defines.hh>

cs::TextureGL4::TextureGL4(cs::eTextureType type)
  : cs::iTexture()
  , m_name(0)
  , m_type(type)
  , m_target(textureTypeMap[type])
  , m_sampler(0)
{
  CS_CLASS_GEN_CONSTR;
}

cs::TextureGL4::~TextureGL4()
{
  if (m_name)
  {
    glDeleteTextures(1, &m_name);
    CS_CHECK_GL_ERROR;
    m_name = 0;
  }
  CS_RELEASE(m_sampler);
  m_sampler = 0;
}

bool cs::TextureGL4::Initialize()
{
  glGenTextures(1, &m_name);
  CS_CHECK_GL_ERROR;
  return m_name != 0;
}

void cs::TextureGL4::SetSampler(cs::SamplerWrapper *sampler)
{
  CS_SET(m_sampler, sampler);
}

cs::SamplerWrapper *cs::TextureGL4::GetSampler()
{
  return m_sampler;
}

const cs::SamplerWrapper *cs::TextureGL4::GetSampler() const
{
  return m_sampler;
}

void cs::TextureGL4::Bind()
{
  glBindTexture(m_target, m_name);
  CS_CHECK_GL_ERROR;
}

cs::eTextureType cs::TextureGL4::GetType() const
{
  return m_type;
}

void cs::TextureGL4::GenerateMipMaps()
{
  Bind();
  glGenerateMipmap(m_target);
  CS_CHECK_GL_ERROR;
}




