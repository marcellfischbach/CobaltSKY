
#include <RenderGL4/Shader.hh>
#include <RenderGL4/MappingGL4.hh>

vkShader::vkShader()
  : m_name(0)
{

}

vkShader::~vkShader()
{
  if (m_name)
  {
    glDeleteShader(m_name);
  }
}

void vkShader::SetSource(const vkString &source)
{
  m_shader = source;
}


const vkString &vkShader::GetSource() const
{
  return m_shader;
}


GLuint vkShader::GetName() const
{
  return m_name;
}

void vkShader::SetShaderType(vkShaderType shaderType)
{
  m_shaderType = shaderType;
}

vkShaderType vkShader::GetShaderType() const
{
  return m_shaderType;
}


bool vkShader::Compile()
{
  m_name = glCreateShader(shaderTypeMap[m_shaderType]);
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

  if (param == GL_FALSE)
  {
    return false;
  }

  return true;
}

vkString vkShader::GetCompileErrorLog() const
{
  GLchar buffer[1024];
  GLsizei length;
  glGetShaderInfoLog(m_name, 1024, &length, buffer);
  if (length < 1024)
  {
    buffer[length] = '\0';
  }

  return vkString(buffer);
}
