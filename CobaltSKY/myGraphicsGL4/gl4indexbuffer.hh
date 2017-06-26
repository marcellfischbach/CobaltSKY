#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/iindexbuffer.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4indexbuffer.refl.hh>

CS_CLASS()
class IndexBufferGL4 : public CS_SUPER(iIndexBuffer)
{
  CS_CLASS_GEN_OBJECT;
public:
  IndexBufferGL4();
  virtual ~IndexBufferGL4();

  bool CreateBuffer(csSize size, const void *data, csBufferDataMode dataMode);

  virtual csSize GetSize() const;

  virtual bool Copy(unsigned offset, csSize size, const void* data);

  virtual bool Lock(unsigned offset, void** data, csBufferAccessMode mode);

  virtual bool Unlock();

  void Bind();
  static void Unbind();

  GLuint GetGLName() const;

private:
  GLuint m_name;
  csSize m_size;
};

