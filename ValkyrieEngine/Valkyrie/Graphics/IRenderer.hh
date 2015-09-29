#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/IRenderer.refl.hh>
#include <Valkyrie/Enums.hh>

struct IIndexBuffer;
struct IVertexBuffer;
struct IVertexDeclaration;
struct IShader;
struct vkVertexElement;

VK_INTERFACE()
struct VKE_API IRenderer : public IObject
{
  VK_CLASS_GEN;


  /** 
   * \name Create of render buffers and render states
   * @{
   */
  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual IVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements) = 0;
  /**
   * @}
   */


  /**
   * \name The rendering API
   * @{
   */
  virtual void SetVertexDeclaration(IVertexDeclaration *vertexDeclaration) = 0;
  virtual void SetVertexBuffer(vkUInt16 streamIdx, IVertexBuffer *vertexBuffer) = 0;
  virtual void SetIndexBuffer(IIndexBuffer *indexBuffer) = 0;
  virtual void SetShader(IShader *shader) = 0;

  virtual void Render(vkPrimitiveType type, vkUInt32 count) = 0;
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType) = 0;
  /**
   * @}
   */

};

