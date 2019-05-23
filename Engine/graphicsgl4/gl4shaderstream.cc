
#include <graphicsgl4/gl4shaderstream.hh>
#include <graphicsgl4/gl4defines.hh>
#include <graphicsgl4/gl4mapping.hh>

cs::ShaderStreamGL4::ShaderStreamGL4()
  : m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  CS_CLASS_GEN_CONSTR;
}

cs::ShaderStreamGL4::~ShaderStreamGL4()
{

}

void cs::ShaderStreamGL4::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &cs::ShaderStreamGL4::GetName() const
{
  return m_name;
}

void cs::ShaderStreamGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool cs::ShaderStreamGL4::IsValid() const
{
  return m_valid;
}

void cs::ShaderStreamGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}


GLint cs::ShaderStreamGL4::GetLocation() const
{
  return m_location;
}

void cs::ShaderStreamGL4::SetArrayIndex(csUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void cs::ShaderStreamGL4::Set(csSize size, csSize stride, csSize offset, cs::eDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, CS_GL_OFFSET(offset));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderStreamGL4::Set(csSize size, csSize stride, const void *data, cs::eDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, (const GLvoid*)data);
  CS_CHECK_GL_ERROR;
}


void cs::ShaderStreamGL4::SetFrequency(csUInt8 frequency)
{
  glVertexAttribDivisor(m_absLocation, frequency);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderStreamGL4::Enable()
{
  glEnableVertexAttribArray(m_absLocation);
  CS_CHECK_GL_ERROR;

}

void cs::ShaderStreamGL4::Disable()
{
  glDisableVertexAttribArray(m_absLocation);
  CS_CHECK_GL_ERROR;

}
