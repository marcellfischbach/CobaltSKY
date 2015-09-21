#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IVertexBuffer.hh>
#include <RenderGL4/VertexBufferGL4.refl.hh>
#include <GL/glew.h>

VK_CLASS();
class VertexBufferGL4 : public IVertexBuffer
{
  VK_CLASS_GEN_OBJECT;
public:
  VertexBufferGL4();
  virtual ~VertexBufferGL4();

  virtual unsigned GetSize() const;

  virtual bool Copy(unsigned offset, unsigned size, const void* data);

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode);

  virtual bool Unlock();

  virtual void Bind();
  virtual void Unbind();

private:
  GLuint m_name;

};

