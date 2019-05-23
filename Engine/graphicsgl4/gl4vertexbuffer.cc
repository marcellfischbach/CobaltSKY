
#include <graphicsgl4/gl4vertexbuffer.hh>
#include <graphicsgl4/gl4defines.hh>
#include <graphicsgl4/gl4mapping.hh>

static GLuint current_mapped_buffer;

VertexBufferGL4::VertexBufferGL4()
  : m_name(0)
{
  CS_CLASS_GEN_CONSTR;
}

VertexBufferGL4::~VertexBufferGL4()
{

}


bool VertexBufferGL4::CreateBuffer(csSize size, const void *data, cs::eBufferDataMode dataMode)
{
  glGenBuffers(1, &m_name);
  if (m_name == 0)
  {
    CS_CHECK_GL_ERROR;
    return false;
  }
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  glBufferData(GL_ARRAY_BUFFER, size, data, bufferDataModeMap[dataMode]);

  CS_GL_FLUSH;
  CS_CHECK_GL_ERROR;

  m_size = size;

  return true;
}

csSize VertexBufferGL4::GetSize() const
{
  return m_size;
}

void VertexBufferGL4::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  CS_CHECK_GL_ERROR;
}

void VertexBufferGL4::Unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  CS_CHECK_GL_ERROR;
}

bool VertexBufferGL4::Copy (unsigned offset, csSize size, const void* data)
{
	if (m_name == 0)
	{
		return false;
	}

  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
	CS_GL_FLUSH;
  CS_CHECK_GL_ERROR;
  return true;
}

bool VertexBufferGL4::Lock(unsigned offset, void **data, cs::eBufferAccessMode mode)
{
  if (m_name == 0 || current_mapped_buffer != 0)
  {
    return false;
  }

  void *res = 0;
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  res = glMapBuffer(GL_ARRAY_BUFFER, bufferAccessModeMap[mode]);
  CS_CHECK_GL_ERROR;
  if (!res)
  {
    CS_GL_FLUSH;
    return false;
  }
  current_mapped_buffer = m_name;
  // now map the offset
  *data = &((char*)res)[offset];
  CS_GL_FLUSH;
  return true;

}


bool VertexBufferGL4::Unlock()
{
  if (m_name == 0 || current_mapped_buffer != m_name)
  {
    CS_GL_FLUSH;
    return false;
  }

  glUnmapBuffer(GL_ARRAY_BUFFER);

  current_mapped_buffer = 0;
  CS_GL_FLUSH;
  CS_CHECK_GL_ERROR;
  return true;
}


GLuint VertexBufferGL4::GetGLName() const
{
  return m_name;
}

