
#pragma once

#include <graphicsgl4/gl4export.hh>
#include <valkyrie/graphics/igraphics.hh>
#include <graphicsgl4/gl4graphics.refl.hh>

class IndexBufferGL4;
class vkProgramGL4;
class vkRenderTargetGL4;
class vkSamplerGL4;
class vkTextureGL4;
class VertexBufferGL4;
class vkVertexDeclarationGL4;

VK_CLASS()
class VKGRAPHICSGL4_API vkGraphicsGL4 : public VK_SUPER(iGraphics)
{
  VK_CLASS_GEN_OBJECT;
public:
  vkGraphicsGL4();
  virtual void ResetDefaults();

  virtual iIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual iVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual iVertexDeclaration *CreateVertexDeclaration(const vkVertexElement *elements);
  virtual iRenderTarget *CreateRenderTarget();
  virtual iSampler *CreateSampler();
  virtual iTexture2D *CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps);
  virtual iTexture2DArray *CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps);
  virtual iTextureCube *CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth);
  virtual iShader *CreateShader(const vkString &vertexCode, const vkString &tessCtrlCode, const vkString &tessEvalCode, const vkString &geometryCode, const vkString &fragmentCode);

  virtual ISGShaderGraphFactory* GetShaderGraphFactory();

  virtual void SetProjectionMatrix(const vkMatrix4f &matrix);
  virtual void SetProjectionMatrixInv(const vkMatrix4f &matrix);
  virtual void SetViewMatrix(const vkMatrix4f &matrix);
  virtual void SetViewMatrixInv(const vkMatrix4f &matrix);
  virtual void SetModelMatrix(const vkMatrix4f &matrix);
  virtual void SetModelMatrixInv(const vkMatrix4f &matrix);
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &out);
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &out);
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &out);
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &out);
  virtual void SetSkeleton(const vkSkeleton *skeleton);
  virtual void SetSkeletonMatrices(const vkMatrix4f *matrices, vkSize numberOfMatrices);
  virtual void SetSkeletonBoneMapping(const vkUInt32 *mapping, vkSize numberOfBoneMappings);

  virtual void SetShadowMatrices(const vkMatrix4f *projView, const vkMatrix4f *proj, const vkMatrix4f *view, const vkVector2f *nearFars, vkSize numberOfMatrices);

  virtual void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration);
  virtual void SetVertexBuffer(vkUInt16 streamIdx, iVertexBuffer *vertexBuffer);
  virtual void SetIndexBuffer(iIndexBuffer *indexBuffer);
  virtual void SetShader(iShader *shader);
  virtual void FreeTextures();
  virtual void InvalidateTextures();
  virtual vkTextureUnit BindTexture(iTexture *texture);
  virtual void SetTexture(vkTextureUnit unit, iTexture *texture);
  virtual void SetSampler(vkTextureUnit unit, iSampler *sampler);
  virtual void SetRenderTarget(iRenderTarget *renderTarget);

  virtual void SetBlendEnabled(bool enabled);
  virtual bool IsBlendEnabled() const;
  virtual void SetBlendMode(vkBlendMode blendSrc, vkBlendMode blendDst);
  virtual void SetBlendMode(vkBlendMode blendSrcColor, vkBlendMode blendDstColor, vkBlendMode blendSrcAlpha, vkBlendMode blendDstAlpha);
  virtual void GetBlendMode(vkBlendMode &blendSrcColor, vkBlendMode &blendDstColor, vkBlendMode &blendSrcAlpha, vkBlendMode &blendDstAlpha) const;

  virtual void SetDepthMask(bool depth);
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha);
  virtual void SetDepthTest(bool depthTest);
  virtual void SetDepthFunc(vkCompareMode compareMode);

  virtual void SetRenderFadeInOut(float near, float far);
  virtual void SetRenderFadeInOutValue(vkUInt8 value);

  virtual void SetRenderDestination(vkRenderDestination renderDestination);
  virtual void SetRenderDestinations(vkRenderDestination *renderDestination, vkSize numRenderDestinations);

  virtual void Clear(bool clearColor = true, const vkVector4f &color = vkVector4f (0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, vkUInt8 stencil = 0);
  virtual void SetViewport(vkUInt16 width, vkUInt16 height);
  virtual void SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height);
  virtual void SetViewport(iRenderTarget *viewport);
  virtual void Render(vkPrimitiveType type, vkUInt32 count);
  virtual void RenderIndexed(vkPrimitiveType type, vkUInt32 count, vkDataType indexDataType);
  virtual void BindValues();

  virtual void RenderFullScreenFrame();
  virtual void RenderFullScreenFrame(iTexture2D *texture);
  virtual void RenderFullScreenFrame(float left, float right, float bottom, float top, iTexture2D *texture);

  virtual void RenderFullScreenFrame(iTexture2DArray *texture, int layer);
  virtual void RenderFullScreenFrame(float left, float right, float bottom, float top, iTexture2DArray *texture, int layer);

  void AssetGraphicsErrors() const;

private:
  void BindMatrices();
  void BindSkeleton();
  bool BindVertexDeclaration();
  void UnbindVertexDeclaration();

  void SetClearColorValue(const vkVector4f &clearColorValue);
  void SetClearDepthValue(float clearDepthValue);
  void SetClearStencilValue(vkUInt8 clearStencilValue);

private:
  void RecalculateMatrix(vkMatrixType type);

  vkVertexDeclarationGL4 *m_vertexDeclaration;
  IndexBufferGL4 *m_indexBuffer;
  VertexBufferGL4 *m_vertexBuffer[16];
  vkProgramGL4 *m_program;
  vkRenderTargetGL4 *m_renderTarget;

  bool m_blendEnabled;
  vkBlendMode m_blendModeSrcColor;
  vkBlendMode m_blendModeSrcAlpha;
  vkBlendMode m_blendModeDstColor;
  vkBlendMode m_blendModeDstAlpha;

  bool m_depthMask;
  vkUInt8 m_colorMask;
  bool m_depthTest;
  vkCompareMode m_depthFunc;


  vkVector4f m_clearColor;
  float m_clearDepth;
  vkUInt8 m_clearStencil;

  vkTextureUnit m_nextTextureUnit;

  vkSamplerGL4 *m_samplers[eTU_COUNT];
  bool m_samplerChanged[eTU_COUNT];

  vkTextureGL4 *m_textures[eTU_COUNT];
  bool m_textureChanged[eTU_COUNT];

  vkMatrix4f m_matrices[eMT_COUNT];
  bool m_matrixNeedsRecalculation[eMT_COUNT];

  const vkMatrix4f *m_skeletonMatrices;
  vkSize m_numberOfSkeletonMatrices;

  const vkUInt32 *m_skeletonBoneMapping;
  vkSize m_numberOfSkeletonBoneMappings;

  vkSize m_numberOfShadowMatrices;
  vkMatrix4f m_shadowMatricesProjView[6];
  vkMatrix4f m_shadowMatricesProj[6];
  vkMatrix4f m_shadowMatricesView[6];
  vkVector2f m_shadowNearFars[6];

  void InvalidateSamplers();

  /**
   * \name Render a full screen frame
   * @{
   */
  void InitFullScreenData();
  VertexBufferGL4 *m_fullScreenVertexBuffer;
  VertexBufferGL4 *m_fullScreenParamVertexBuffer;
  vkVertexDeclarationGL4 *m_fullScreenVertexDeclaration;
  vkProgramGL4 *m_fullScreenProgram;
  vkProgramGL4 *m_fullScreenArrayProgram;
  vkUInt16 m_viewportWidth;
  vkUInt16 m_viewportHeight;
  vkVector3f m_fadeInOutDistances;
  vkUInt8 m_fadeInOutValue;
  /**
   * @}
   */

  ISGShaderGraphFactory* m_shaderGraphFactory;
};
