
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <RenderGL4/RendererGL4.refl.hh>

class IndexBufferGL4;
class vkProgramGL4;
class vkSamplerGL4;
class vkTextureGL4;
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


  virtual void SetProjectionMatrix(const vkMatrix4f &matrix);
  virtual void SetViewMatrix(const vkMatrix4f &matrix);
  virtual void SetModelMatrix(const vkMatrix4f &matrix);

  virtual void SetVertexDeclaration(IVertexDeclaration *vertexDeclaration);
  virtual void SetVertexBuffer(vkUInt16 streamIdx, IVertexBuffer *vertexBuffer);
  virtual void SetIndexBuffer(IIndexBuffer *indexBuffer);
  virtual void SetShader(IShader *shader);
  virtual void SetTexture(vkTextureUnit unit, ITexture *texture);
  virtual void SetSampler(vkTextureUnit unit, ISampler *sampler);

  virtual void Clear();
  virtual void SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height);
  virtual void Render(vkPrimitiveType type, vkUInt32 count);
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType);

private:
  void BindMatrices();
  bool BindVertexDeclaration();
  void UnbindVertexDeclaration();

private:
  void RecalculateMatrix(vkMatrixType type);

  vkVertexDeclarationGL4 *m_vertexDeclaration;
  IndexBufferGL4 *m_indexBuffer;
  VertexBufferGL4 *m_vertexBuffer[16];
  vkProgramGL4 *m_program;

  vkSamplerGL4 *m_samplers[eTU_COUNT];
  bool m_samplerChanged[eTU_COUNT];

  vkTextureGL4 *m_textures[eTU_COUNT];
  bool m_textureChanged[eTU_COUNT];

  vkMatrix4f m_matrices[eMT_COUNT];
  bool m_matrixNeedsRecalculation[eMT_COUNT];

  void InvalidateSamplers();
  void InvalidateTextures();

};

