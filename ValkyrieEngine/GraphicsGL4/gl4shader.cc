
#include <GraphicsGL4/gl4shader.hh>
#include <GraphicsGL4/gl4mapping.hh>



vkShaderGL4::vkShaderGL4()
  : m_name(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkShaderGL4::~vkShaderGL4()
{
  if (m_name)
  {
    glDeleteShader(m_name);
  }
}

void vkShaderGL4::SetSource(const vkString &source)
{
  m_shader = source;
}


const vkString &vkShaderGL4::GetSource() const
{
  return m_shader;
}


GLuint vkShaderGL4::GetName() const
{
  return m_name;
}

void vkShaderGL4::SetShaderType(vkShaderType shaderType)
{
  m_shaderType = shaderType;
}

vkShaderType vkShaderGL4::GetShaderType() const
{
  return m_shaderType;
}


bool vkShaderGL4::Compile()
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

vkString vkShaderGL4::GetCompileErrorLog() const
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







