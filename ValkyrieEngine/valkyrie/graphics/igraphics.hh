#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/math/vkmatrix.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/igraphics.refl.hh>

struct iFrameProcessor;
struct iRenderTarget;
struct iIndexBuffer;
struct iVertexBuffer;
struct iVertexDeclaration;
struct iSampler;
struct ISGShaderGraphFactory;
struct iShader;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct vkVertexElement;
class vkSkeleton;

#ifdef _DEBUG 
#	define VK_CHECK_GRAPHICS_ERROR(graphics) if (graphics) graphics->AssetGraphicsErrors()
#else
#	define VK_CHECK_GRAPHICS_ERROR(graphics)
#endif


VK_INTERFACE()
struct VKE_API iGraphics : public iObject
{
  VK_CLASS_GEN;

  virtual void ResetDefaults() = 0;

  /**
   * \name Create of render buffers and render states
   * @{
   */
  virtual iIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual iVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual iVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements) = 0;
  virtual iRenderTarget *CreateRenderTarget() = 0;
  virtual iSampler *CreateSampler() = 0;
  virtual iTexture2D *CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps) = 0;
  virtual iTexture2DArray *CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps) = 0;
  virtual iTextureCube *CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth) = 0;
  virtual iShader *CreateShader(const vkString &vertexCode, const vkString &tessCtrl, const vkString &tessEval, const vkString &geometry, const vkString &fragmentCode) = 0;
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
  virtual void SetSkeletonBoneMapping(const vkUInt32 *mapping, vkSize numberOfMappings) = 0;
  virtual void SetShadowMatrices(const vkMatrix4f *projView, const vkMatrix4f *proj, const vkMatrix4f *view, const vkVector2f *nearFars, vkSize numberOfMatrices) = 0;
  /**
   * @}
   */

   /**
    * \name The rendering API
    * @{
    */
  virtual void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration) = 0;
  virtual void SetVertexBuffer(vkUInt16 streamIdx, iVertexBuffer *vertexBuffer) = 0;
  virtual void SetIndexBuffer(iIndexBuffer *indexBuffer) = 0;
  virtual void SetShader(iShader *shader) = 0;
  virtual void FreeTextures() = 0;
  virtual void InvalidateTextures() = 0;
  virtual vkTextureUnit BindTexture(iTexture *texture) = 0;
  virtual void SetTexture(vkTextureUnit unit, iTexture *texture) = 0;
  virtual void SetSampler(vkTextureUnit unit, iSampler *sampler) = 0;
  virtual void SetRenderTarget(iRenderTarget *renderTarget) = 0;

  virtual void SetBlendEnabled(bool enabled) = 0;
  virtual bool IsBlendEnabled() const = 0;
  virtual void SetBlendMode(vkBlendMode blendSrc, vkBlendMode blendDst) = 0;
  virtual void SetBlendMode(vkBlendMode blendSrcColor, vkBlendMode blendDstColor, vkBlendMode blendSrcAlpha, vkBlendMode blendDstAlpha) = 0;
  virtual void GetBlendMode(vkBlendMode &blendSrcColor, vkBlendMode &blendDstColor, vkBlendMode &blendSrcAlpha, vkBlendMode &blendDstAlpha) const = 0;

  virtual void SetDepthMask(bool depth) = 0;
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) = 0;
  virtual void SetDepthTest(bool depthTest) = 0;
  virtual void SetDepthFunc(vkCompareMode compareMode) = 0;



  virtual void SetRenderFadeInOut(float near, float far) = 0;
  virtual void SetRenderFadeInOutValue(vkUInt8 value) = 0;

  virtual void SetRenderDestination(vkRenderDestination renderDestination) = 0;
  virtual void SetRenderDestinations(vkRenderDestination *renderDestination, vkSize numRenderDestinations) = 0;

  virtual void Clear(bool clearColor = true, const vkVector4f &color = vkVector4f(0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, vkUInt8 stencil = 0) = 0;
  virtual void SetViewport(vkUInt16 width, vkUInt16 height) = 0;
  virtual void SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height) = 0;
  virtual void SetViewport(iRenderTarget *viewport) = 0;
  virtual void Render(vkPrimitiveType type, vkUInt32 count) = 0;
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType) = 0;
  virtual void RenderFullScreenFrame() = 0;
  virtual void RenderFullScreenFrame(iTexture2D* textureColor) = 0;
  virtual void BindValues() = 0;
  /**
   * @}
   */

  virtual void AssetGraphicsErrors() const = 0;
};

