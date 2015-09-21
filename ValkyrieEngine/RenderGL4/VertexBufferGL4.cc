
#include <RenderGL4/VertexBufferGL4.hh>
#include <RenderGL4/DefinesGL4.hh>
#include <RenderGL4/MappingGL4.hh>

static GLuint current_mapped_buffer;

VertexBufferGL4::VertexBufferGL4()
{
  glGenBuffers(1, &m_name);
}

VertexBufferGL4::~VertexBufferGL4()
{

}

void VertexBufferGL4::Bind()
{
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
}

void VertexBufferGL4::Unbind()
{
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

bool VertexBufferGL4::Lock(unsigned offset, void **data, vkBufferAccessMode mode)
{
  if (m_name == 0 || current_mapped_buffer != 0)
  {
    return false;
  }

  void *res = 0;
  glBindBuffer(GL_ARRAY_BUFFER, m_name);
  res = glMapBuffer(GL_ARRAY_BUFFER, bufferAccessModeMap[mode]);
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