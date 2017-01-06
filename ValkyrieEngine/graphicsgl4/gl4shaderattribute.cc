
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4mapping.hh>




vkShaderAttributeGL4::vkShaderAttributeGL4()
  : IShaderAttribute()
  , m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  VK_CLASS_GEN_CONSTR;
}


vkShaderAttributeGL4::~vkShaderAttributeGL4()
{
}

void vkShaderAttributeGL4::SetName(const vkString &name)
{
  m_name = name;
}

const vkString &vkShaderAttributeGL4::GetName() const
{
  return m_name;
}

void vkShaderAttributeGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool vkShaderAttributeGL4::IsValid() const
{
  return m_valid;
}

void vkShaderAttributeGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}

GLint vkShaderAttributeGL4::GetLocation() const
{
  return m_location;
}

void vkShaderAttributeGL4::SetArrayIndex(vkUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void vkShaderAttributeGL4::Set(float x)
{
  glUniform1f(m_absLocation, x);
}

void vkShaderAttributeGL4::Set(float x, float y)
{
  glUniform2f(m_absLocation, x, y);
}

void vkShaderAttributeGL4::Set(float x, float y, float z)
{
  glUniform3f(m_absLocation, x, y, z);
}

void vkShaderAttributeGL4::Set(float x, float y, float z, float w)
{
  glUniform4f(m_absLocation, x, y, z, w);
}

void vkShaderAttributeGL4::Set(vkInt32 x)
{
  glUniform1i(m_absLocation, x);
}

void vkShaderAttributeGL4::Set(vkInt32 x, vkInt32 y)
{
  glUniform2i(m_absLocation, x, y);
}

void vkShaderAttributeGL4::Set(vkInt32 x, vkInt32 y, vkInt32 z)
{
  glUniform3i(m_absLocation, x, y, z);
}

void vkShaderAttributeGL4::Set(vkInt32 x, vkInt32 y, vkInt32 z, vkInt32 w)
{
  glUniform4i(m_absLocation, x, y, z, w);
}

void vkShaderAttributeGL4::Set(const vkVector2f &v)
{
  glUniform2f(m_absLocation, v.x, v.y);
}

void vkShaderAttributeGL4::Set(const vkVector3f &v)
{
  glUniform3f(m_absLocation, v.x, v.y, v.z);
}

void vkShaderAttributeGL4::Set(const vkVector4f &v)
{
  glUniform4f(m_absLocation, v.x, v.y, v.z, v.w);
}


void vkShaderAttributeGL4::Set(const vkColor4f &v)
{
  glUniform4f(m_absLocation, v.r, v.g, v.b, v.a);
}


void vkShaderAttributeGL4::Set(const vkVector2f *vs, vkSize num)
{
  glUniform2fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void vkShaderAttributeGL4::Set(const vkVector3f *vs, vkSize num)
{
  glUniform3fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void vkShaderAttributeGL4::Set(const vkVector4f *vs, vkSize num)
{
  glUniform4fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void vkShaderAttributeGL4::Set(const vkInt32 *vs, vkSize num)
{
  glUniform1iv(m_absLocation, num, reinterpret_cast<const GLint*>(vs));
}

void vkShaderAttributeGL4::Set(const vkUInt32 *vs, vkSize num)
{
  glUniform1uiv(m_absLocation, num, reinterpret_cast<const GLuint*>(vs));
}

void vkShaderAttributeGL4::Set(const float *vs, vkSize num)
{
  glUniform1fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void vkShaderAttributeGL4::Set(const vkMatrix3f &m)
{
  glUniformMatrix3fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
}

void vkShaderAttributeGL4::Set(const vkMatrix3f *ms, vkSize num)
{
  glUniformMatrix3fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
}

void vkShaderAttributeGL4::Set(const vkMatrix4f &m)
{
  glUniformMatrix4fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
}

void vkShaderAttributeGL4::Set(const vkMatrix4f *ms, vkSize num)
{
  glUniformMatrix4fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
}