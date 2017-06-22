
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>



csShaderAttributeGL4::csShaderAttributeGL4()
  : iShaderAttribute()
  , m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  CS_CLASS_GEN_CONSTR;
}


csShaderAttributeGL4::~csShaderAttributeGL4()
{
}

void csShaderAttributeGL4::SetName(const csString &name)
{
  m_name = name;
}

const csString &csShaderAttributeGL4::GetName() const
{
  return m_name;
}

void csShaderAttributeGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool csShaderAttributeGL4::IsValid() const
{
  return m_valid;
}

void csShaderAttributeGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}

GLint csShaderAttributeGL4::GetLocation() const
{
  return m_location;
}

void csShaderAttributeGL4::SetArrayIndex(csUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void csShaderAttributeGL4::Set(float x)
{
  CS_CHECK_GL_ERROR;
  glUniform1f(m_absLocation, x);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(float x, float y)
{
  CS_CHECK_GL_ERROR;
  glUniform2f(m_absLocation, x, y);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(float x, float y, float z)
{
  CS_CHECK_GL_ERROR;
  glUniform3f(m_absLocation, x, y, z);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(float x, float y, float z, float w)
{
  CS_CHECK_GL_ERROR;
  glUniform4f(m_absLocation, x, y, z, w);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(csInt32 x)
{
  CS_CHECK_GL_ERROR;
  glUniform1i(m_absLocation, x);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y)
{
  CS_CHECK_GL_ERROR;
  glUniform2i(m_absLocation, x, y);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z)
{
  CS_CHECK_GL_ERROR;
  glUniform3i(m_absLocation, x, y, z);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w)
{
  CS_CHECK_GL_ERROR;
  glUniform4i(m_absLocation, x, y, z, w);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csVector2f &v)
{
  CS_CHECK_GL_ERROR;
  glUniform2f(m_absLocation, v.x, v.y);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csVector3f &v)
{
  CS_CHECK_GL_ERROR;
  glUniform3f(m_absLocation, v.x, v.y, v.z);
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csVector4f &v)
{
  CS_CHECK_GL_ERROR;
  glUniform4f(m_absLocation, v.x, v.y, v.z, v.w);
  CS_CHECK_GL_ERROR;
}


void csShaderAttributeGL4::Set(const csColor4f &v)
{
  CS_CHECK_GL_ERROR;
  glUniform4f(m_absLocation, v.r, v.g, v.b, v.a);
  CS_CHECK_GL_ERROR;
}


void csShaderAttributeGL4::Set(const csVector2f *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform2fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csVector3f *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform3fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csVector4f *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform4fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csInt32 *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform1iv(m_absLocation, num, reinterpret_cast<const GLint*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csUInt32 *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform1uiv(m_absLocation, num, reinterpret_cast<const GLuint*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const float *vs, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniform1fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csMatrix3f &m)
{
  CS_CHECK_GL_ERROR;
  glUniformMatrix3fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csMatrix3f *ms, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniformMatrix3fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csMatrix4f &m)
{
  CS_CHECK_GL_ERROR;
  glUniformMatrix4fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
  CS_CHECK_GL_ERROR;
}

void csShaderAttributeGL4::Set(const csMatrix4f *ms, csSize num)
{
  CS_CHECK_GL_ERROR;
  glUniformMatrix4fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
  CS_CHECK_GL_ERROR;
}