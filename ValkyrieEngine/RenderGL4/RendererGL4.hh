
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <RenderGL4/RendererGL4.refl.hh>

class IndexBufferGL4;
class vkProgramGL4;
class VertexBufferGL4;
class vkVertexDeclarationGL4;

VK_CLASS();
class VKGL4_API RendererGL4 : public IRenderer
{
  VK_CLASS_GEN_OBJECT;
public:
  RendererGL4();

  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual IVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements);

  virtual void SetVertexDeclaration(IVertexDeclaration *vertexDeclaration);
  virtual void SetVertexBuffer(vkUInt16 streamIdx, IVertexBuffer *vertexBuffer);
  virtual void SetIndexBuffer(IIndexBuffer *indexBuffer);
  virtual void SetShader(IShader *shader);

  virtual void Render(vkPrimitiveType type, vkUInt32 count);
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType);

private:
  bool BindVertexDeclaration();
  void UnbindVertexDeclaration();

private:

  vkVertexDeclarationGL4 *m_vertexDeclaration;
  IndexBufferGL4 *m_indexBuffer;
  VertexBufferGL4 *m_vertexBuffer[16];
  vkProgramGL4 *m_program;

};

