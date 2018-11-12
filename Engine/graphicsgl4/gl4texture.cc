

#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4defines.hh>

csTextureGL4::csTextureGL4(csTextureType type)
  : iTexture()
  , m_name(0)
  , m_type(type)
  , m_target(textureTypeMap[type])
  , m_sampler(0)
{
  CS_CLASS_GEN_CONSTR;
}

csTextureGL4::~csTextureGL4()
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

bool csTextureGL4::Initialize()
{
  glGenTextures(1, &m_name);
  CS_CHECK_GL_ERROR;
  return m_name != 0;
}

void csTextureGL4::SetSampler(csSamplerWrapper *sampler)
{
  CS_SET(m_sampler, sampler);
}

csSamplerWrapper *csTextureGL4::GetSampler()
{
  return m_sampler;
}

const csSamplerWrapper *csTextureGL4::GetSampler() const
{
  return m_sampler;
}

void csTextureGL4::Bind()
{
  glBindTexture(m_target, m_name);
  CS_CHECK_GL_ERROR;
}

csTextureType csTextureGL4::GetType() const
{
  return m_type;
}

void csTextureGL4::GenerateMipMaps()
{
  Bind();
  glGenerateMipmap(m_target);
  CS_CHECK_GL_ERROR;
}




