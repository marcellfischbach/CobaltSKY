
#include <RenderGL4/Shader.hh>
#include <RenderGL4/MappingGL4.hh>


#define VK_GL_OFFSET(offset)  ((const GLvoid*)((unsigned long)offset))

vkShaderGL4::vkShaderGL4()
  : m_name(0)
{

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










vkShaderAttributeGL4::vkShaderAttributeGL4()
  : IShaderAttribute()
  , m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
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




vkShaderStreamGL4::vkShaderStreamGL4()
  : m_name("")
  , m_location(-1)
  , m_arrayIndex(0)
  , m_absLocation(-1)
{

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



vkProgramGL4::vkProgramGL4()
  : IShader()
  , m_name(0)
{
  VK_CLASS_GEN_CONSTR;
}

vkProgramGL4::~vkProgramGL4()
{
  for (auto shader : m_shaders)
  {
    glDetachShader(m_name, shader->GetName());
    shader->Release();
  }
  m_shaders.clear();

  glDeleteProgram(m_name);
}


void vkProgramGL4::Bind()
{
  glUseProgram(m_name);
}

void vkProgramGL4::RegisterAttribute(vkShaderAttributeID &id)
{
  ResizeAttributes(id.GetID());

  
  GLint loc = glGetUniformLocation(m_name, id.ResolveName().c_str());
  vkShaderAttributeGL4 &attribute = m_attributes[id.GetID()];
  attribute.SetLocation(loc);
  attribute.SetName(id.GetName());
}

void vkProgramGL4::RegisterStream(vkShaderStreamID &id)
{
  ResizeStreams(id.GetID());


  GLint loc = glGetAttribLocation(m_name, id.ResolveName().c_str());
  vkShaderStreamGL4 &stream = m_streams[id.GetID()];
  stream.SetLocation(loc);
  stream.SetName(id.GetName());
}


vkUInt32 vkProgramGL4::GetNumberOfAttributes() const
{
  return (vkUInt32)m_attributes.size();
}

IShaderAttribute *vkProgramGL4::GetAttribute(const vkShaderAttributeID &id)
{
  if (id.GetID() >= m_attributes.size())
  {
    return 0;
  }

  vkShaderAttributeGL4 &attr = m_attributes[id.GetID()];
  if (attr.GetLocation() == -1)
  {
    return 0;
  }
  
  return &attr;
}



vkUInt16 vkProgramGL4::GetNumberOfStreams() const
{
  return (vkUInt16)m_streams.size();
}

IShaderStream *vkProgramGL4::GetStream(const vkShaderStreamID &id)
{
  if (id.GetID() >= m_streams.size())
  {
    return 0;
  }

  vkShaderStreamGL4 &stream = m_streams[id.GetID()];
  if (stream.GetLocation() == -1)
  {
    return 0;
  }

  return &stream;
}

void vkProgramGL4::AttachShader(vkShaderGL4 *shader)
{
  if (m_name != 0)
  {
    printf("Cannot attach shader to already created program\n");
    return;
  }
  if (shader)
  {
    shader->AddRef();
    m_shaders.push_back(shader);
  }
}


void vkProgramGL4::DetachShader(vkShaderGL4 *shader)
{
  if (m_name != 0)
  {
    printf("Cannot detach shader from already created program\n");
    return;
  }
  if (shader)
  {
    for (std::vector<vkShaderGL4*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
      if (*it == shader)
      {
        m_shaders.erase(it);
        shader->Release();
        return;
      }
    }
  }
}

bool vkProgramGL4::Link()
{
  if (m_name != 0)
  {
    glDeleteProgram(m_name);
  }

  m_name = glCreateProgram();

  for (auto shader : m_shaders)
  {
    glAttachShader(m_name, shader->GetName());
  }

  glLinkProgram(m_name);

  GLint param;
  glGetProgramiv(m_name, GL_LINK_STATUS, &param);

  if (param == GL_FALSE)
  {
    return false;
  }

  return true;

}

vkString vkProgramGL4::GetLinkErrorLog() const
{
  GLchar buffer[1024];
  GLsizei length;
  glGetProgramInfoLog(m_name, 1024, &length, buffer);
  if (length < 1024)
  {
    buffer[length] = '\0';
  }

  return vkString(buffer);
}



void vkProgramGL4::ResizeAttributes(vkUInt32 id)
{
  vkInt32 missing = (vkInt32)(id - m_attributes.size() + 1);
  for (vkInt16 i = 0; i < missing; ++i)
  {
    m_attributes.push_back(vkShaderAttributeGL4());
  }
}


void vkProgramGL4::ResizeStreams(vkUInt32 id)
{
  vkInt32 missing = (vkInt32)(id - m_streams.size() + 1);
  for (vkInt16 i = 0; i < missing; ++i)
  {
    m_streams.push_back(vkShaderStreamGL4());
  }
}
