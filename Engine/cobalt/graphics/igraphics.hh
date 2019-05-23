#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/math/csmatrix.hh>
#include <cobalt/csenums.hh>
#include <string>
#include <cobalt/graphics/igraphics.refl.hh>


namespace cs
{
class Skeleton;
struct iIndexBuffer;
struct iFrameProcessor;
struct iSampler;
struct iSGShaderGraphFactory;
struct iRenderTarget;
struct iShader;
struct iTexture;
struct iTexture2D;
struct iTexture2DArray;
struct iTextureCube;
struct iVertexBuffer;
struct iVertexDeclaration;
struct VertexElement;

#ifdef _DEBUG 
#	define CS_CHECK_GRAPHICS_ERROR(graphics) if (graphics) graphics->AssetGraphicsErrors()
#else
#	define CS_CHECK_GRAPHICS_ERROR(graphics)
#endif


CS_CLASS()
struct CSE_API iGraphics : public cs::iObject
{
  CS_CLASS_GEN;

  virtual void ResetDefaults() = 0;

  /**
   * \name Create of render buffers and render states
   * @{
   */
  virtual cs::iIndexBuffer *CreateIndexBuffer(csSize size, const void *data, cs::eBufferDataMode mode) = 0;
  virtual cs::iVertexBuffer *CreateVertexBuffer(csSize size, const void *data, cs::eBufferDataMode mode) = 0;
  virtual cs::iVertexDeclaration *CreateVertexDeclaration(const cs::VertexElement *elements) = 0;
  virtual cs::iRenderTarget *CreateRenderTarget() = 0;
  virtual cs::iSampler *CreateSampler() = 0;
  virtual cs::iTexture2D *CreateTexture2D(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps) = 0;
  virtual cs::iTexture2DArray *CreateTexture2DArray(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps) = 0;
  virtual cs::iTextureCube *CreateTextureCube(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth) = 0;
  virtual cs::iShader *CreateShader(const std::string &vertexCode, const std::string &tessCtrl, const std::string &tessEval, const std::string &geometry, const std::string &fragmentCode) = 0;
  /**
   * @}
   */
  virtual cs::iSGShaderGraphFactory* GetShaderGraphFactory() = 0;

   /**
    * \name Transformation handling
    * @{
    */
  virtual void SetProjectionMatrix(const cs::Matrix4f &matrix) = 0;
  virtual void SetProjectionMatrixInv(const cs::Matrix4f &matrix) = 0;
  virtual void SetViewMatrix(const cs::Matrix4f &matrix) = 0;
  virtual void SetViewMatrixInv(const cs::Matrix4f &matrix) = 0;
  virtual void SetModelMatrix(const cs::Matrix4f &matrix) = 0;
  virtual void SetModelMatrixInv(const cs::Matrix4f &matrix) = 0;
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f &out) = 0;
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f &out) = 0;
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f &out) = 0;
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f &out) = 0;
  virtual void SetSkeleton(const cs::Skeleton *skeleton) = 0;
  virtual void SetSkeletonMatrices(const cs::Matrix4f *matrices, csSize numberOfMatrices) = 0;
  virtual void SetSkeletonBoneMapping(const csUInt32 *mapping, csSize numberOfMappings) = 0;
  virtual void SetShadowMatrices(const cs::Matrix4f *projView, const cs::Matrix4f *proj, const cs::Matrix4f *view, const cs::Vector2f *nearFars, csSize numberOfMatrices) = 0;
  /**
   * @}
   */

   /**
    * \name The rendering API
    * @{
    */
  virtual void SetVertexDeclaration(cs::iVertexDeclaration *vertexDeclaration) = 0;
  virtual void SetVertexBuffer(csUInt16 streamIdx, cs::iVertexBuffer *vertexBuffer) = 0;
  virtual void SetIndexBuffer(cs::iIndexBuffer *indexBuffer) = 0;
  virtual void SetShader(cs::iShader *shader) = 0;
  virtual void FreeTextures() = 0;
  virtual void InvalidateTextures() = 0;
  virtual cs::eTextureUnit BindTexture(cs::iTexture *texture) = 0;
  virtual void SetTexture(cs::eTextureUnit unit, cs::iTexture *texture) = 0;
  virtual void SetSampler(cs::eTextureUnit unit, cs::iSampler *sampler) = 0;
  virtual void SetRenderTarget(cs::iRenderTarget *renderTarget) = 0;

  virtual void PushRenderStates() = 0;
  virtual void PopRenderStates() = 0;
  virtual void SetBlendEnabled(bool enabled) = 0;
  virtual bool IsBlendEnabled() const = 0;
  virtual void SetBlendMode(cs::eBlendMode blendSrc, cs::eBlendMode blendDst) = 0;
  virtual void SetBlendMode(cs::eBlendMode blendSrcColor, cs::eBlendMode blendDstColor, cs::eBlendMode blendSrcAlpha, cs::eBlendMode blendDstAlpha) = 0;
  virtual void GetBlendMode(cs::eBlendMode &blendSrcColor, cs::eBlendMode &blendDstColor, cs::eBlendMode &blendSrcAlpha, cs::eBlendMode &blendDstAlpha) const = 0;

  virtual void SetDepthMask(bool depth) = 0;
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha) = 0;
  virtual void SetDepthTest(bool depthTest) = 0;
  virtual void SetDepthFunc(cs::eCompareMode compareMode) = 0;

  virtual void SetFrontFace(cs::eFaceWinding faceWinding) = 0;
  virtual cs::eFaceWinding GetFrontFace() const = 0;
  virtual void SetCullFaceEnabled(bool enable) = 0;
  virtual bool IsCullFaceEnabled() const = 0;
  virtual void SetCullFace(cs::eFaceSide side) = 0;
  virtual cs::eFaceSide GetCullFace() const = 0;

  virtual void SetFillMode(cs::eFillMode fillMode) = 0;
  virtual cs::eFillMode GetFillMode() const = 0;

  virtual void SetRenderFadeInOut(float near, float far) = 0;
  virtual void SetRenderFadeInOutValue(csUInt8 value) = 0;

  virtual void SetRenderDestination(cs::eRenderDestination renderDestination) = 0;
  virtual void SetRenderDestinations(cs::eRenderDestination *renderDestination, csSize numRenderDestinations) = 0;

  virtual void Clear(bool clearColor = true, const cs::Vector4f &color = cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, csUInt8 stencil = 0) = 0;
  virtual void SetViewport(csUInt16 width, csUInt16 height) = 0;
  virtual void SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height) = 0;
  virtual void SetViewport(cs::iRenderTarget *viewport) = 0;
  virtual void Render(cs::ePrimitiveType type, csUInt32 count) = 0;
  virtual void RenderIndexed(cs::ePrimitiveType type, csUInt32 count, cs::eDataType indexDataType) = 0;
  virtual void RenderFullScreenFrame() = 0;
  virtual void RenderFullScreenFrame(cs::iTexture2D* textureColor) = 0;
  virtual void BindValues() = 0;
  /**
   * @}
   */

  virtual void AssetGraphicsErrors() const = 0;
};

}
