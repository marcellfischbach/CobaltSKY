#pragma once

#include <GraphicsGL4/Export.hh>
#include <Valkyrie/graphics/iindexbuffer.hh>
#include <GL/glew.h>
#include <GraphicsGL4/IndexBufferGL4.refl.hh>

VK_CLASS()
class IndexBufferGL4 : public VK_SUPER(IIndexBuffer)
{
  VK_CLASS_GEN_OBJECT;
public:
  IndexBufferGL4();
  virtual ~IndexBufferGL4();

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

