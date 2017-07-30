#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csstring.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/igraphics.refl.hh>

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
struct csVertexElement;
class csSkeleton;

#ifdef _DEBUG 
#	define CS_CHECK_GRAPHICS_ERROR(graphics) if (graphics) graphics->AssetGraphicsErrors()
#else
#	define CS_CHECK_GRAPHICS_ERROR(graphics)
#endif


CS_INTERFACE()
struct CSE_API iGraphics : public iObject
{
  CS_CLASS_GEN;

  virtual void ResetDefaults() = 0;

  /**
   * \name Create of render buffers and render states
   * @{
   */
  virtual iIndexBuffer *CreateIndexBuffer(csSize size, const void *data, csBufferDataMode mode) = 0;
  virtual iVertexBuffer *CreateVertexBuffer(csSize size, const void *data, csBufferDataMode mode) = 0;
  virtual iVertexDeclaration *CreateVertexDeclaration(const csVertexElement *elements) = 0;
  virtual iRenderTarget *CreateRenderTarget() = 0;
  virtual iSampler *CreateSampler() = 0;
  virtual iTexture2D *CreateTexture2D(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps) = 0;
  virtual iTexture2DArray *CreateTexture2DArray(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps) = 0;
  virtual iTextureCube *CreateTextureCube(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth) = 0;
  virtual iShader *CreateShader(const csString &vertexCode, const csString &tessCtrl, const csString &tessEval, const csString &geometry, const csString &fragmentCode) = 0;
  /**
   * @}
   */
  virtual ISGShaderGraphFactory* GetShaderGraphFactory() = 0;

   /**
    * \name Transformation handling
    * @{
    */
  virtual void SetProjectionMatrix(const csMatrix4f &matrix) = 0;
  virtual void SetProjectionMatrixInv(const csMatrix4f &matrix) = 0;
  virtual void SetViewMatrix(const csMatrix4f &matrix) = 0;
  virtual void SetViewMatrixInv(const csMatrix4f &matrix) = 0;
  virtual void SetModelMatrix(const csMatrix4f &matrix) = 0;
  virtual void SetModelMatrixInv(const csMatrix4f &matrix) = 0;
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &out) = 0;
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &out) = 0;
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &out) = 0;
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &out) = 0;
  virtual void SetSkeleton(const csSkeleton *skeleton) = 0;
  virtual void SetSkeletonMatrices(const csMatrix4f *matrices, csSize numberOfMatrices) = 0;
  virtual void SetSkeletonBoneMapping(const csUInt32 *mapping, csSize numberOfMappings) = 0;
  virtual void SetShadowMatrices(const csMatrix4f *projView, const csMatrix4f *proj, const csMatrix4f *view, const csVector2f *nearFars, csSize numberOfMatrices) = 0;
  /**
   * @}
   */

   /**
    * \name The rendering API
    * @{
    */
  virtual void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration) = 0;
  virtual void SetVertexBuffer(csUInt16 streamIdx, iVertexBuffer *vertexBuffer) = 0;
  virtual void SetIndexBuffer(iIndexBuffer *indexBuffer) = 0;
  virtual void SetShader(iShader *shader) = 0;
  virtual void FreeTextures() = 0;
  virtual void InvalidateTextures() = 0;
  virtual csTextureUnit BindTexture(iTexture *texture) = 0;
  virtual void SetTexture(csTextureUnit unit, iTexture *texture) = 0;
  virtual void SetSampler(csTextureUnit unit, iSampler *sampler) = 0;
  virtual void SetRenderTarget(iRenderTarget *renderTarget) = 0;

  virtual void SetBlendEnabled(bool enabled) = 0;
  virtual bool IsBlendEnabled() const = 0;
  virtual void SetBlendMode(csBlendMode blendSrc, csBlendMode blendDst) = 0;
  virtual void SetBlendMode(csBlendMode blendSrcColor, csBlendMode blendDstColor, csBlendMode blendSrcAlpha, csBlendMode blendDstAlpha) = 0;
  virtual void GetBlendMode(csBlendMode &blendSrcColor, csBlendMode &blendDstColor, csBlendMode &blendSrcAlpha, csBlendMode &blendDstAlpha) const = 0;

  virtual void SetDepthMask(bool depth) = 0;
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) = 0;
  virtual void SetDepthTest(bool depthTest) = 0;
  virtual void SetDepthFunc(csCompareMode compareMode) = 0;

  virtual void SetFrontFace(csFaceWinding faceWinding) = 0;
  virtual csFaceWinding GetFrontFace() const = 0;
  virtual void SetCullFaceEnabled(bool enable) = 0;
  virtual bool IsCullFaceEnabled() const = 0;
  virtual void SetCullFace(csFaceSide side) = 0;
  virtual csFaceSide GetCullFace() const = 0;


  virtual void SetRenderFadeInOut(float near, float far) = 0;
  virtual void SetRenderFadeInOutValue(csUInt8 value) = 0;

  virtual void SetRenderDestination(csRenderDestination renderDestination) = 0;
  virtual void SetRenderDestinations(csRenderDestination *renderDestination, csSize numRenderDestinations) = 0;

  virtual void Clear(bool clearColor = true, const csVector4f &color = csVector4f(0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, csUInt8 stencil = 0) = 0;
  virtual void SetViewport(csUInt16 width, csUInt16 height) = 0;
  virtual void SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height) = 0;
  virtual void SetViewport(iRenderTarget *viewport) = 0;
  virtual void Render(csPrimitiveType type, csUInt32 count) = 0;
  virtual void RenderIndexed(csPrimitiveType type, csUInt32 count, csDataType indexDataType) = 0;
  virtual void RenderFullScreenFrame() = 0;
  virtual void RenderFullScreenFrame(iTexture2D* textureColor) = 0;
  virtual void BindValues() = 0;
  /**
   * @}
   */

  virtual void AssetGraphicsErrors() const = 0;
};

