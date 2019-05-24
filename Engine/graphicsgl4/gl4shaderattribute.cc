
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>



cs::ShaderAttributeGL4::ShaderAttributeGL4()
  : cs::iShaderAttribute()
  , m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
  , m_valid(false)
{
  CS_CLASS_GEN_CONSTR;
}


cs::ShaderAttributeGL4::~ShaderAttributeGL4()
{
}

void cs::ShaderAttributeGL4::SetName(const std::string &name)
{
  m_name = name;
}

const std::string &cs::ShaderAttributeGL4::GetName() const
{
  return m_name;
}

void cs::ShaderAttributeGL4::SetValid(bool valid)
{
  m_valid = valid;
}

bool cs::ShaderAttributeGL4::IsValid() const
{
  return m_valid;
}

void cs::ShaderAttributeGL4::SetLocation(GLint location)
{
  m_location = location;
  m_absLocation = m_location + m_arrayIndex;
}

GLint cs::ShaderAttributeGL4::GetLocation() const
{
  return m_location;
}

void cs::ShaderAttributeGL4::SetArrayIndex(csUInt32 arrayIndex)
{
  m_arrayIndex = arrayIndex;
  m_absLocation = m_location + m_arrayIndex;
}

void cs::ShaderAttributeGL4::Set(float x)
{
  glUniform1f(m_absLocation, x);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(float x, float y)
{
  glUniform2f(m_absLocation, x, y);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(float x, float y, float z)
{
  glUniform3f(m_absLocation, x, y, z);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(float x, float y, float z, float w)
{
  glUniform4f(m_absLocation, x, y, z, w);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(csInt32 x)
{
  glUniform1i(m_absLocation, x);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(csInt32 x, csInt32 y)
{
  glUniform2i(m_absLocation, x, y);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z)
{
  glUniform3i(m_absLocation, x, y, z);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w)
{
  glUniform4i(m_absLocation, x, y, z, w);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Vector2f &v)
{
  glUniform2f(m_absLocation, v.x, v.y);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Vector3f &v)
{
  glUniform3f(m_absLocation, v.x, v.y, v.z);
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Vector4f &v)
{
  glUniform4f(m_absLocation, v.x, v.y, v.z, v.w);
  CS_CHECK_GL_ERROR;
}


void cs::ShaderAttributeGL4::Set(const cs::Color4f &v)
{
  glUniform4f(m_absLocation, v.r, v.g, v.b, v.a);
  CS_CHECK_GL_ERROR;
}


void cs::ShaderAttributeGL4::Set(const cs::Vector2f *vs, csSize num)
{
  glUniform2fv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Vector3f *vs, csSize num)
{
  glUniform3fv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Vector4f *vs, csSize num)
{
  glUniform4fv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const csInt32 *vs, csSize num)
{
  glUniform1iv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLint*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const csUInt32 *vs, csSize num)
{
  glUniform1uiv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLuint*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const float *vs, csSize num)
{
  glUniform1fv(m_absLocation, static_cast<GLsizei>(num), reinterpret_cast<const GLfloat*>(vs));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Matrix3f &m)
{
  glUniformMatrix3fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Matrix3f *ms, csSize num)
{
  glUniformMatrix3fv(m_absLocation, static_cast<GLsizei>(num), false, reinterpret_cast<const GLfloat*>(ms));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Matrix4f &m)
{
  glUniformMatrix4fv(m_absLocation, 1, false, static_cast<const GLfloat*>(&m.m00));
  CS_CHECK_GL_ERROR;
}

void cs::ShaderAttributeGL4::Set(const cs::Matrix4f *ms, csSize num)
{
  glUniformMatrix4fv(m_absLocation, static_cast<GLsizei>(num), false, reinterpret_cast<const GLfloat*>(ms));
  CS_CHECK_GL_ERROR;
}