
#include <GraphicsGL4/gl4shaderstream.hh>
#include <GraphicsGL4/gl4defines.hh>
#include <GraphicsGL4/gl4mapping.hh>

vkShaderStreamGL4::vkShaderStreamGL4()
  : m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderStreamGL4::~vkShaderStreamGL4()
{

}

void vkShaderStreamGL4::SetName(const vkString &name)
{
  m_name = name;
}

const vkString &vkShaderStreamGL4::GetName() const
{
  return m_name;
}

void vkShaderStreamGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool vkShaderStreamGL4::IsValid() const
{
  return m_valid;
}

void vkShaderStreamGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}


GLint vkShaderStreamGL4::GetLocation() const
{
  return m_location;
}

void vkShaderStreamGL4::SetArrayIndex(vkUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void vkShaderStreamGL4::Set(vkSize size, vkSize stride, vkSize offset, vkDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, VK_GL_OFFSET(offset));
}

void vkShaderStreamGL4::Set(vkSize size, vkSize stride, const void *data, vkDataType type)
{
  glVertexAttribPointer(m_absLocation, (GLsizei)size, dataTypeMap[type], false, (GLsizei)stride, (const GLvoid*)data);
}


void vkShaderStreamGL4::SetFrequency(vkUInt8 frequency)
{
  glVertexAttribDivisor(m_absLocation, frequency);
}

void vkShaderStreamGL4::Enable()
{
  glEnableVertexAttribArray(m_absLocation);

}

void vkShaderStreamGL4::Disable()
{
  glDisableVertexAttribArray(m_absLocation);

}
