
#include <graphicsgl4/gl4shaderstream.hh>
#include <graphicsgl4/gl4defines.hh>
#include <graphicsgl4/gl4mapping.hh>

csShaderStreamGL4::csShaderStreamGL4()
  : m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  CS_CLASS_GEN_CONSTR;
}

csShaderStreamGL4::~csShaderStreamGL4()
{

}

void csShaderStreamGL4::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &csShaderStreamGL4::GetName() const
{
  return m_name;
}

void csShaderStreamGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool csShaderStreamGL4::IsValid() const
{
  return m_valid;
}

void csShaderStreamGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}


GLint csShaderStreamGL4::GetLocation() const
{
  return m_location;
}

void csShaderStreamGL4::SetArrayIndex(csUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void csShaderStreamGL4::Set(csSize size, csSize stride, csSize offset, cs::eDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, CS_GL_OFFSET(offset));
  CS_CHECK_GL_ERROR;
}

void csShaderStreamGL4::Set(csSize size, csSize stride, const void *data, cs::eDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, (const GLvoid*)data);
  CS_CHECK_GL_ERROR;
}


void csShaderStreamGL4::SetFrequency(csUInt8 frequency)
{
  glVertexAttribDivisor(m_absLocation, frequency);
  CS_CHECK_GL_ERROR;
}

void csShaderStreamGL4::Enable()
{
  glEnableVertexAttribArray(m_absLocation);
  CS_CHECK_GL_ERROR;

}

void csShaderStreamGL4::Disable()
{
  glDisableVertexAttribArray(m_absLocation);
  CS_CHECK_GL_ERROR;

}
