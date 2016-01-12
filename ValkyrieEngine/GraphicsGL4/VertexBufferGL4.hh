#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <GraphicsGL4/VertexBufferGL4.refl.hh>
#include <GL/glew.h>

VK_CLASS();
class VertexBufferGL4 : public IVertexBuffer
{
  VK_CLASS_GEN_OBJECT;
public:
  VertexBufferGL4();
  virtual ~VertexBufferGL4();

  bool CreateBuffer(vkSize size, const void *data, vkBufferDataMode dataMode);

  virtual vkSize GetSize() const;

  virtual bool Copy(unsigned offset, vkSize size, const void* data);

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode);

  virtual bool Unlock();

  void Bind();
  static void Unbind();

  GLuint GetGLName() const;

private:
  GLuint m_name;
  vkSize m_size;
};
