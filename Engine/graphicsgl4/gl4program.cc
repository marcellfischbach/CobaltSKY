
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4shaderstream.hh>
#include <graphicsgl4/gl4defines.hh>




csProgramGL4::csProgramGL4()
  : iShader()
  , m_name(0)
{
  CS_CLASS_GEN_CONSTR;
}

csProgramGL4::~csProgramGL4()
{
  for (auto shader : m_shaders)
  {
    if (m_name)
    {
      glDetachShader(m_name, shader->Get()->GetName());
      CS_CHECK_GL_ERROR;
    }
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
  CS_CHECK_GL_ERROR;
}


void csProgramGL4::Bind()
{
  if (!glIsProgram(m_name))
  {
    return;
  }
  glUseProgram(m_name);
  CS_CHECK_GL_ERROR;
}

void csProgramGL4::InitializeSystemAttributes()
{
  for (csUInt32 i = 0; i < eVAT_COUNT; ++i)
  {
    RegisterAttribute(csShaderAttributeID(i));
  }
}

void csProgramGL4::InitializeSystemStreams()
{
  for (csUInt32 i = 0; i < eVST_COUNT; ++i)
  {
    RegisterStream(csShaderStreamID(i));
  }

}

void csProgramGL4::RegisterAttribute(const csShaderAttributeID &id)
{
  ResizeAttributes(id.GetID());

  std::string uniformName = std::string("cs_") + id.GetName();
  GLint loc = glGetUniformLocation(m_name, uniformName.c_str());
  csShaderAttributeGL4 *attribute = m_attributes[id.GetID()];
  attribute->SetLocation(loc);
  attribute->SetName(id.GetName());
  attribute->SetValid(true);
  CS_CHECK_GL_ERROR;

}

void csProgramGL4::RegisterStream(const csShaderStreamID &id)
{
  ResizeStreams(id.GetID());

  std::string attribName = std::string("cs_") + id.GetName();
  GLint loc = glGetAttribLocation(m_name, attribName.c_str());
  csShaderStreamGL4 *stream = m_streams[id.GetID()];
  stream->SetLocation(loc);
  stream->SetName(id.GetName());
  stream->SetValid(true);
  CS_CHECK_GL_ERROR;
}


csUInt32 csProgramGL4::GetNumberOfAttributes() const
{
  return (csUInt32)m_attributes.size();
}

iShaderAttribute *csProgramGL4::GetAttribute(csUInt32 idx)
{
  if (idx >= m_attributes.size())
  {
    return 0;
  }
  csShaderAttributeGL4 *attr = m_attributes[idx];
  if (attr->GetLocation() == -1)
  {
    return 0;
  }

  return attr;
}

iShaderAttribute *csProgramGL4::GetAttribute(const csShaderAttributeID &id)
{
  if (id.GetID() >= m_attributes.size())
  {
    ResizeAttributes(id.GetID());
  }

  csShaderAttributeGL4 *attr = m_attributes[id.GetID()];
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

iShaderAttribute *csProgramGL4::GetAttribute(const std::string &name)
{
  for (size_t i = 0, in = m_namedAttributes.size(); i < in; ++i)
  {
    if (m_namedAttributes[i]->GetName() == name)
    {
      return m_namedAttributes[i];
    }
  }

  Bind();
  std::string uniformName = std::string("cs_") + name;
  GLint loc = glGetUniformLocation(m_name, uniformName.c_str());
  CS_CHECK_GL_ERROR;
  if (loc == -1)
  {
    return 0;
  }

  csShaderAttributeGL4 *attribute = new csShaderAttributeGL4();
  attribute->SetLocation(loc);
  attribute->SetName(name);
  attribute->SetValid(true);

  m_namedAttributes.push_back(attribute);
  return attribute;
}



csUInt16 csProgramGL4::GetNumberOfStreams() const
{
  return (csUInt16)m_streams.size();
}

iShaderStream *csProgramGL4::GetStream(const csShaderStreamID &id)
{
  if (id.GetID() >= m_streams.size())
  {
    ResizeStreams(id.GetID());
  }

  csShaderStreamGL4 *stream = m_streams[id.GetID()];
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


iShaderStream *csProgramGL4::GetStream(const std::string &name)
{
  for (size_t i = 0, in = m_namedStreams.size(); i < in; ++i)
  {
    if (m_namedStreams[i]->GetName() == name)
    {
      return m_namedStreams[i];
    }
  }

  Bind();
  std::string attribName = std::string("cs_") + name;
  GLint loc = glGetAttribLocation(m_name, attribName.c_str());
  CS_CHECK_GL_ERROR;
  if (loc == -1)
  {
    return 0;
  }
  csShaderStreamGL4 *stream = new csShaderStreamGL4();
  stream->SetLocation(loc);
  stream->SetName(name);
  stream->SetValid(true);

  m_namedStreams.push_back(stream);
  return stream;
}



void csProgramGL4::AttachShader(csShaderGL4Wrapper *shader)
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


void csProgramGL4::DetachShader(csShaderGL4Wrapper *shader)
{
  if (m_name != 0)
  {
    printf("Cannot detach shader from already created program\n");
    return;
  }
  if (shader)
  {
    for (std::vector<csShaderGL4Wrapper*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
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

bool csProgramGL4::Link()
{
  if (m_name != 0)
  {
    glDeleteProgram(m_name);
    CS_CHECK_GL_ERROR;
  }

  m_name = glCreateProgram();

  CS_CHECK_GL_ERROR;
  for (auto shader : m_shaders)
  {
    glAttachShader(m_name, shader->Get()->GetName());
  }

  glLinkProgram(m_name);

  GLint param;
  glGetProgramiv(m_name, GL_LINK_STATUS, &param);
  CS_CHECK_GL_ERROR;

  if (param == GL_FALSE)
  {
    for (auto shader : m_shaders)
    {
      glDetachShader(m_name, shader->Get()->GetName());
    }
    glDeleteProgram(m_name);
    m_name = 0;
    return false;
  }

  InitializeSystemAttributes();
  InitializeSystemStreams();

  return true;

}

std::string csProgramGL4::GetLinkErrorLog() const
{
  GLchar buffer[1024];
  GLsizei length;
  glGetProgramInfoLog(m_name, 1024, &length, buffer);
  CS_CHECK_GL_ERROR;
  if (length < 1024)
  {
    buffer[length] = '\0';
  }

  return std::string(buffer);
}



void csProgramGL4::ResizeAttributes(csUInt32 id)
{
  csInt32 missing = (csInt32)(id - m_attributes.size() + 1);
  for (csInt16 i = 0; i < missing; ++i)
  {
    m_attributes.push_back(new csShaderAttributeGL4());
  }
}


void csProgramGL4::ResizeStreams(csUInt32 id)
{
  csInt32 missing = (csInt32)(id - m_streams.size() + 1);
  for (csInt16 i = 0; i < missing; ++i)
  {
    m_streams.push_back(new csShaderStreamGL4());
  }
}
