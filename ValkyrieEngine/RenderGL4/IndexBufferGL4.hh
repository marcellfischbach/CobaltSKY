#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IIndexBuffer.hh>
#include <RenderGL4/IndexBufferGL4.refl.hh>
#include <GL/glew.h>

VK_CLASS();
class IndexBufferGL4 : public IIndexBuffer
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

  virtual void Bind();
  virtual void Unbind();

  GLuint GetGLName() const;

private:
  GLuint m_name;
  vkSize m_size;
};

