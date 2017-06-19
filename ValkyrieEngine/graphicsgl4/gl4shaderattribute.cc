
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4mapping.hh>




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
  glUniform1f(m_absLocation, x);
}

void csShaderAttributeGL4::Set(float x, float y)
{
  glUniform2f(m_absLocation, x, y);
}

void csShaderAttributeGL4::Set(float x, float y, float z)
{
  glUniform3f(m_absLocation, x, y, z);
}

void csShaderAttributeGL4::Set(float x, float y, float z, float w)
{
  glUniform4f(m_absLocation, x, y, z, w);
}

void csShaderAttributeGL4::Set(csInt32 x)
{
  glUniform1i(m_absLocation, x);
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y)
{
  glUniform2i(m_absLocation, x, y);
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z)
{
  glUniform3i(m_absLocation, x, y, z);
}

void csShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w)
{
  glUniform4i(m_absLocation, x, y, z, w);
}

void csShaderAttributeGL4::Set(const csVector2f &v)
{
  glUniform2f(m_absLocation, v.x, v.y);
}

void csShaderAttributeGL4::Set(const csVector3f &v)
{
  glUniform3f(m_absLocation, v.x, v.y, v.z);
}

void csShaderAttributeGL4::Set(const csVector4f &v)
{
  glUniform4f(m_absLocation, v.x, v.y, v.z, v.w);
}


void csShaderAttributeGL4::Set(const csColor4f &v)
{
  glUniform4f(m_absLocation, v.r, v.g, v.b, v.a);
}


void csShaderAttributeGL4::Set(const csVector2f *vs, csSize num)
{
  glUniform2fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void csShaderAttributeGL4::Set(const csVector3f *vs, csSize num)
{
  glUniform3fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void csShaderAttributeGL4::Set(const csVector4f *vs, csSize num)
{
  glUniform4fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void csShaderAttributeGL4::Set(const csInt32 *vs, csSize num)
{
  glUniform1iv(m_absLocation, num, reinterpret_cast<const GLint*>(vs));
}

void csShaderAttributeGL4::Set(const csUInt32 *vs, csSize num)
{
  glUniform1uiv(m_absLocation, num, reinterpret_cast<const GLuint*>(vs));
}

void csShaderAttributeGL4::Set(const float *vs, csSize num)
{
  glUniform1fv(m_absLocation, num, reinterpret_cast<const GLfloat*>(vs));
}

void csShaderAttributeGL4::Set(const csMatrix3f &m)
{
  glUniformMatrix3fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
}

void csShaderAttributeGL4::Set(const csMatrix3f *ms, csSize num)
{
  glUniformMatrix3fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
}

void csShaderAttributeGL4::Set(const csMatrix4f &m)
{
  glUniformMatrix4fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
}

void csShaderAttributeGL4::Set(const csMatrix4f *ms, csSize num)
{
  glUniformMatrix4fv(m_absLocation, num, false, reinterpret_cast<const GLfloat*>(ms));
}