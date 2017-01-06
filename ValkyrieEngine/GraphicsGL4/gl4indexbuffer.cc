
#include <GraphicsGL4/gl4indexbuffer.hh>
#include <GraphicsGL4/gl4defines.hh>
#include <GraphicsGL4/gl4mapping.hh>

static GLuint current_mapped_buffer;

IndexBufferGL4::IndexBufferGL4()
  : m_name(0)
{
  VK_CLASS_GEN_CONSTR;
}

IndexBufferGL4::~IndexBufferGL4()
{

}


bool IndexBufferGL4::CreateBuffer(vkSize size, const void *data, vkBufferDataMode dataMode)
{
  glGenBuffers(1, &m_name);
  if (m_name == 0)
  {
    return false;
  }
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_name);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, bufferDataModeMap[dataMode]);

  VK_GL_FLUSH;
  VK_CHECK_GL_ERROR;

  m_size = size;

  return true;
}

vkSize IndexBufferGL4::GetSize() const
{
  return m_size;
}

void IndexBufferGL4::Bind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_name);
}

void IndexBufferGL4::Unbind()
{
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool IndexBufferGL4::Copy(unsigned offset, vkSize size, const void* data)
{
  if (m_name == 0)
  {
    return false;
  }

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_name);
  glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
  VK_GL_FLUSH;
  VK_CHECK_GL_ERROR;
  return true;
}

bool IndexBufferGL4::Lock(unsigned offset, void **data, vkBufferAccessMode mode)
{
  if (m_name == 0 || current_mapped_buffer != 0)
  {
    return false;
  }

  void *res = 0;
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_name);
  res = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferAccessModeMap[mode]);
  VK_CHECK_GL_ERROR;
  if (!res)
  {
    VK_GL_FLUSH;
    return false;
  }
  current_mapped_buffer = m_name;
  // now map the offset
  *data = &((char*)res)[offset];
  VK_GL_FLUSH;
  return true;

}


bool IndexBufferGL4::Unlock()
{
  if (m_name == 0 || current_mapped_buffer != m_name)
  {
    VK_GL_FLUSH;
    return false;
  }

  glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

  current_mapped_buffer = 0;
  VK_GL_FLUSH;
  VK_CHECK_GL_ERROR;
  return true;
}


GLuint IndexBufferGL4::GetGLName() const
{
  return m_name;
}

