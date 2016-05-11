#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Math/Matrix.hh>
#include <Valkyrie/Enums.hh>
#include <Valkyrie/Graphics/IGraphics.refl.hh>

struct IFrameProcessor;
struct IRenderTarget;
struct IIndexBuffer;
struct IVertexBuffer;
struct IVertexDeclaration;
struct ISampler;
struct ISGShaderGraphFactory;
struct IShader;
struct ITexture;
struct ITexture2D;
struct ITexture2DArray;
struct ITextureCube;
struct vkVertexElement;
class vkSkeleton;

VK_INTERFACE()
struct VKE_API IGraphics : public IObject
{
  VK_CLASS_GEN;

  virtual void ResetDefaults() = 0;

  /**
   * \name Create of render buffers and render states
   * @{
   */
  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual IVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements) = 0;
  virtual IRenderTarget *CreateRenderTarget() = 0;
  virtual ISampler *CreateSampler() = 0;
  virtual ITexture2D *CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height) = 0;
  virtual ITexture2DArray *CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers) = 0;
  virtual ITextureCube *CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth) = 0;
  virtual IFrameProcessor *CreateDeferredFrameProcessor() = 0;
  /**
   * @}
   */
  virtual ISGShaderGraphFactory* GetShaderGraphFactory() = 0;

   /**
    * \name Transformation handling
    * @{
    */
  virtual void SetProjectionMatrix(const vkMatrix4f &matrix) = 0;
  virtual void SetProjectionMatrixInv(const vkMatrix4f &matrix) = 0;
  virtual void SetViewMatrix(const vkMatrix4f &matrix) = 0;
  virtual void SetViewMatrixInv(const vkMatrix4f &matrix) = 0;
  virtual void SetModelMatrix(const vkMatrix4f &matrix) = 0;
  virtual void SetModelMatrixInv(const vkMatrix4f &matrix) = 0;
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &out) = 0;
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &out) = 0;
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &out) = 0;
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &out) = 0;
  virtual void SetSkeleton(const vkSkeleton *skeleton) = 0;
  virtual void SetSkeletonMatrices(const vkMatrix4f *matrices, vkSize numberOfMatrices) = 0;

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
  virtual void FreeTextures() = 0;
  virtual void InvalidateTextures() = 0;
  virtual vkTextureUnit BindTexture(ITexture *texture) = 0;
  virtual void SetTexture(vkTextureUnit unit, ITexture *texture) = 0;
  virtual void SetSampler(vkTextureUnit unit, ISampler *sampler) = 0;
  virtual void SetRenderTarget(IRenderTarget *renderTarget) = 0;

  virtual void SetBlendEnabled(bool enabled) = 0;
  virtual bool IsBlendEnabled() const = 0;
  virtual void SetBlendMode(vkBlendMode blendSrc, vkBlendMode blendDst) = 0;
  virtual void SetBlendMode(vkBlendMode blendSrcColor, vkBlendMode blendDstColor, vkBlendMode blendSrcAlpha, vkBlendMode blendDstAlpha) = 0;
  virtual void GetBlendMode(vkBlendMode &blendSrcColor, vkBlendMode &blendDstColor, vkBlendMode &blendSrcAlpha, vkBlendMode &blendDstAlpha) const = 0;

  virtual void SetRenderFadeInOut(float near, float far) = 0;
  virtual void SetRenderFadeInOutValue(vkUInt8 value) = 0;

  virtual void Clear(bool clearColor = true, const vkVector4f &color = vkVector4f(0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, vkUInt8 stencil = 0) = 0;
  virtual void SetViewport(vkUInt16 width, vkUInt16 height) = 0;
  virtual void SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height) = 0;
  virtual void SetViewport(IRenderTarget *viewport) = 0;
  virtual void Render(vkPrimitiveType type, vkUInt32 count) = 0;
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType) = 0;
  virtual void RenderFullScreenFrame() = 0;
  virtual void RenderFullScreenFrame(ITexture2D* textureColor) = 0;
  virtual void BindValues() = 0;
  /**
   * @}
   */

};

