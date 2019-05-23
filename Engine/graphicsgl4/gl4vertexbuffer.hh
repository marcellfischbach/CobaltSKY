#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/ivertexbuffer.hh>
#include <GL/glew.h>
#include <graphicsgl4/gl4vertexbuffer.refl.hh>

CS_CLASS();
class VertexBufferGL4 : public CS_SUPER(cs::iVertexBuffer)
{
  CS_CLASS_GEN_OBJECT;
public:
  VertexBufferGL4();
  virtual ~VertexBufferGL4();

  bool CreateBuffer(csSize size, const void *data, cs::eBufferDataMode dataMode);

  virtual csSize GetSize() const;

  virtual bool Copy(unsigned offset, csSize size, const void* data);

  virtual bool Lock(unsigned offset, void** data, cs::eBufferAccessMode mode);

  virtual bool Unlock();

  void Bind();
  static void Unbind();

  GLuint GetGLName() const;

private:
  GLuint m_name;
  csSize m_size;
};

