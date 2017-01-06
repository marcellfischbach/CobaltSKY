
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4shaderstream.hh>




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

  for (auto stream : m_streams)
  {
    stream->Release();
  }
  for (auto stream : m_namedStreams)
  {
    stream->Release();
  }

  for (auto attribute : m_attributes)
  {
    attribute->Release();
  }
  for (auto attribute : m_namedAttributes)
  {
    attribute->Release();
  }

  glDeleteProgram(m_name);
}


void vkProgramGL4::Bind()
{
  glUseProgram(m_name);
}

void vkProgramGL4::InitializeSystemAttributes()
{
  for (vkUInt32 i = 0; i < eVAT_COUNT; ++i)
  {
    RegisterAttribute(vkShaderAttributeID(i));
  }
}

void vkProgramGL4::InitializeSystemStreams()
{
  for (vkUInt32 i = 0; i < eVST_COUNT; ++i)
  {
    RegisterStream(vkShaderStreamID(i));
  }

}

void vkProgramGL4::RegisterAttribute(const vkShaderAttributeID &id)
{
  ResizeAttributes(id.GetID());

  vkString uniformName = vkString("vk_") + id.GetName();
  GLint loc = glGetUniformLocation(m_name, uniformName.c_str());
  vkShaderAttributeGL4 *attribute = m_attributes[id.GetID()];
  attribute->SetLocation(loc);
  attribute->SetName(id.GetName());
  attribute->SetValid(true);
}

void vkProgramGL4::RegisterStream(const vkShaderStreamID &id)
{
  ResizeStreams(id.GetID());

  vkString attribName = vkString("vk_") + id.GetName();
  GLint loc = glGetAttribLocation(m_name, attribName.c_str());
  vkShaderStreamGL4 *stream = m_streams[id.GetID()];
  stream->SetLocation(loc);
  stream->SetName(id.GetName());
  stream->SetValid(true);
}


vkUInt32 vkProgramGL4::GetNumberOfAttributes() const
{
  return (vkUInt32)m_attributes.size();
}

IShaderAttribute *vkProgramGL4::GetAttribute(vkUInt32 idx)
{
  if (idx >= m_attributes.size())
  {
    return 0;
  }
  vkShaderAttributeGL4 *attr = m_attributes[idx];
  if (attr->GetLocation() == -1)
  {
    return 0;
  }

  return attr;
}

IShaderAttribute *vkProgramGL4::GetAttribute(const vkShaderAttributeID &id)
{
  if (id.GetID() >= m_attributes.size())
  {
    ResizeAttributes(id.GetID());
  }

  vkShaderAttributeGL4 *attr = m_attributes[id.GetID()];
  if (!attr->IsValid())
  {
    RegisterAttribute(id);
  }
  if (attr->GetLocation() == -1)
  {
    return 0;
  }

  return attr;
}

IShaderAttribute *vkProgramGL4::GetAttribute(const vkString &name)
{
  for (size_t i = 0, in = m_namedAttributes.size(); i < in; ++i)
  {
    if (m_namedAttributes[i]->GetName() == name)
    {
      return m_namedAttributes[i];
    }
  }

  Bind();
  vkString uniformName = vkString("vk_") + name;
  GLint loc = glGetUniformLocation(m_name, uniformName.c_str());
  if (loc == -1)
  {
    return 0;
  }

  vkShaderAttributeGL4 *attribute = new vkShaderAttributeGL4();
  attribute->SetLocation(loc);
  attribute->SetName(name);
  attribute->SetValid(true);

  m_namedAttributes.push_back(attribute);
  return attribute;
}



vkUInt16 vkProgramGL4::GetNumberOfStreams() const
{
  return (vkUInt16)m_streams.size();
}

IShaderStream *vkProgramGL4::GetStream(const vkShaderStreamID &id)
{
  if (id.GetID() >= m_streams.size())
  {
    ResizeStreams(id.GetID());
  }

  vkShaderStreamGL4 *stream = m_streams[id.GetID()];
  if (!stream->IsValid())
  {
    RegisterStream(id);
  }
  if (stream->GetLocation() == -1)
  {
    return 0;
  }

  return stream;
}


IShaderStream *vkProgramGL4::GetStream(const vkString &name)
{
  for (size_t i = 0, in = m_namedStreams.size(); i < in; ++i)
  {
    if (m_namedStreams[i]->GetName() == name)
    {
      return m_namedStreams[i];
    }
  }

  Bind();
  vkString attribName = vkString("vk_") + name;
  GLint loc = glGetAttribLocation(m_name, attribName.c_str());
  if (loc == -1)
  {
    return 0;
  }
  vkShaderStreamGL4 *stream = new vkShaderStreamGL4();
  stream->SetLocation(loc);
  stream->SetName(name);
  stream->SetValid(true);

  m_namedStreams.push_back(stream);
  return stream;
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

  InitializeSystemAttributes();
  InitializeSystemStreams();

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
    m_attributes.push_back(new vkShaderAttributeGL4());
  }
}


void vkProgramGL4::ResizeStreams(vkUInt32 id)
{
  vkInt32 missing = (vkInt32)(id - m_streams.size() + 1);
  for (vkInt16 i = 0; i < missing; ++i)
  {
    m_streams.push_back(new vkShaderStreamGL4());
  }
}
