
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>


csShaderGL4::csShaderGL4()
  : m_name(0)
{
  CS_CLASS_GEN_CONSTR;
}

csShaderGL4::~csShaderGL4()
{
  if (m_name)
  {
    glDeleteShader(m_name);
    CS_CHECK_GL_ERROR;
  }
}

void csShaderGL4::SetSource(const csString &source)
{
  m_shader = source;
}


const csString &csShaderGL4::GetSource() const
{
  return m_shader;
}


GLuint csShaderGL4::GetName() const
{
  return m_name;
}

void csShaderGL4::SetShaderType(csShaderType shaderType)
{
  m_shaderType = shaderType;
}

csShaderType csShaderGL4::GetShaderType() const
{
  return m_shaderType;
}


bool csShaderGL4::Compile()
{
  m_name = glCreateShader(shaderTypeMap[m_shaderType]);
  CS_CHECK_GL_ERROR;
  if (!m_name)
  {
    return false;
  }

  GLint length = (GLint)m_shader.length();
  const GLchar *code = static_cast<const GLchar*>(m_shader.c_str());
  glShaderSource(m_name, 1, &code, &length);

  glCompileShader(m_name);

  GLint param;
  glGetShaderiv(m_name, GL_COMPILE_STATUS, &param);
  CS_CHECK_GL_ERROR;

  if (param == GL_FALSE)
  {
    return false;
  }

  return true;
}

csString csShaderGL4::GetCompileErrorLog() const
{
  GLchar buffer[1024];
  GLsizei length;
  glGetShaderInfoLog(m_name, 1024, &length, buffer);
  CS_CHECK_GL_ERROR;
  if (length < 1024)
  {
    buffer[length] = '\0';
  }

  return csString(buffer);
}







