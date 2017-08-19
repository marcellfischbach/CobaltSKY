
#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/igraphics.hh>
#include <graphicsgl4/gl4graphics.refl.hh>

class IndexBufferGL4;
class csProgramGL4;
class csRenderTargetGL4;
class csSamplerGL4;
class csTextureGL4;
class VertexBufferGL4;
class csVertexDeclarationGL4;

CS_CLASS()
class CSGRAPHICSGL4_API csGraphicsGL4 : public CS_SUPER(iGraphics)
{
  CS_CLASS_GEN_OBJECT;
public:
  csGraphicsGL4();
  virtual void ResetDefaults();

  virtual iIndexBuffer *CreateIndexBuffer(csSize size, const void *data, csBufferDataMode mode);
  virtual iVertexBuffer *CreateVertexBuffer(csSize size, const void *data, csBufferDataMode mode);
  virtual iVertexDeclaration *CreateVertexDeclaration(const csVertexElement *elements);
  virtual iRenderTarget *CreateRenderTarget();
  virtual iSampler *CreateSampler();
  virtual iTexture2D *CreateTexture2D(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps);
  virtual iTexture2DArray *CreateTexture2DArray(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps);
  virtual iTextureCube *CreateTextureCube(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);
  virtual iShader *CreateShader(const csString &vertexCode, const csString &tessCtrlCode, const csString &tessEvalCode, const csString &geometryCode, const csString &fragmentCode);

  virtual ISGShaderGraphFactory* GetShaderGraphFactory();

  virtual void SetProjectionMatrix(const csMatrix4f &matrix);
  virtual void SetProjectionMatrixInv(const csMatrix4f &matrix);
  virtual void SetViewMatrix(const csMatrix4f &matrix);
  virtual void SetViewMatrixInv(const csMatrix4f &matrix);
  virtual void SetModelMatrix(const csMatrix4f &matrix);
  virtual void SetModelMatrixInv(const csMatrix4f &matrix);
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &out);
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &out);
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &out);
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &out);
  virtual void SetSkeleton(const csSkeleton *skeleton);
  virtual void SetSkeletonMatrices(const csMatrix4f *matrices, csSize numberOfMatrices);
  virtual void SetSkeletonBoneMapping(const csUInt32 *mapping, csSize numberOfBoneMappings);

  virtual void SetShadowMatrices(const csMatrix4f *projView, const csMatrix4f *proj, const csMatrix4f *view, const csVector2f *nearFars, csSize numberOfMatrices);

  virtual void SetVertexDeclaration(iVertexDeclaration *vertexDeclaration);
  virtual void SetVertexBuffer(csUInt16 streamIdx, iVertexBuffer *vertexBuffer);
  virtual void SetIndexBuffer(iIndexBuffer *indexBuffer);
  virtual void SetShader(iShader *shader);
  virtual void FreeTextures();
  virtual void InvalidateTextures();
  virtual csTextureUnit BindTexture(iTexture *texture);
  virtual void SetTexture(csTextureUnit unit, iTexture *texture);
  virtual void SetSampler(csTextureUnit unit, iSampler *sampler);
  virtual void SetRenderTarget(iRenderTarget *renderTarget);

  virtual void PushRenderStates();
  virtual void PopRenderStates();
  virtual void SetBlendEnabled(bool enabled);
  virtual bool IsBlendEnabled() const;
  virtual void SetBlendMode(csBlendMode blendSrc, csBlendMode blendDst);
  virtual void SetBlendMode(csBlendMode blendSrcColor, csBlendMode blendDstColor, csBlendMode blendSrcAlpha, csBlendMode blendDstAlpha);
  virtual void GetBlendMode(csBlendMode &blendSrcColor, csBlendMode &blendDstColor, csBlendMode &blendSrcAlpha, csBlendMode &blendDstAlpha) const;

  virtual void SetDepthMask(bool depth);
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha);
  virtual void SetDepthTest(bool depthTest);
  virtual void SetDepthFunc(csCompareMode compareMode);

  virtual void SetFrontFace(csFaceWinding faceWinding);
  virtual csFaceWinding GetFrontFace() const;
  virtual void SetCullFaceEnabled(bool enable);
  virtual bool IsCullFaceEnabled() const;
  virtual void SetCullFace(csFaceSide side);
  virtual csFaceSide GetCullFace() const;

  virtual void SetFillMode(csFillMode fillMode);
  virtual csFillMode GetFillMode() const;

  virtual void SetRenderFadeInOut(float near, float far);
  virtual void SetRenderFadeInOutValue(csUInt8 value);

  virtual void SetRenderDestination(csRenderDestination renderDestination);
  virtual void SetRenderDestinations(csRenderDestination *renderDestination, csSize numRenderDestinations);

  virtual void Clear(bool clearColor = true, const csVector4f &color = csVector4f (0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, csUInt8 stencil = 0);
  virtual void SetViewport(csUInt16 width, csUInt16 height);
  virtual void SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height);
  virtual void SetViewport(iRenderTarget *viewport);
  virtual void Render(csPrimitiveType type, csUInt32 count);
  virtual void RenderIndexed(csPrimitiveType type, csUInt32 count, csDataType indexDataType);
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

  void SetClearColorValue(const csVector4f &clearColorValue);
  void SetClearDepthValue(float clearDepthValue);
  void SetClearStencilValue(csUInt8 clearStencilValue);

private:
  void RecalculateMatrix(csMatrixType type);

  csUInt32 m_vao;

  csVertexDeclarationGL4 *m_vertexDeclaration;
  IndexBufferGL4 *m_indexBuffer;
  VertexBufferGL4 *m_vertexBuffer[16];
  csProgramGL4 *m_program;
  csRenderTargetGL4 *m_renderTarget;

  bool m_blendEnabled;
  csBlendMode m_blendModeSrcColor;
  csBlendMode m_blendModeSrcAlpha;
  csBlendMode m_blendModeDstColor;
  csBlendMode m_blendModeDstAlpha;

  bool m_depthMask;
  csUInt8 m_colorMask;
  bool m_depthTest;
  csCompareMode m_depthFunc;

  csVector4f m_clearColor;
  float m_clearDepth;
  csUInt8 m_clearStencil;

  csFaceWinding m_frontFace;
  csFaceSide m_cullFace;
  bool m_cullFaceEnabled;

  csFillMode m_fillMode;

  csTextureUnit m_nextTextureUnit;

  csSamplerGL4 *m_samplers[eTU_COUNT];
  bool m_samplerChanged[eTU_COUNT];

  csTextureGL4 *m_textures[eTU_COUNT];
  bool m_textureChanged[eTU_COUNT];

  csMatrix4f m_matrices[eMT_COUNT];
  bool m_matrixNeedsRecalculation[eMT_COUNT];

  const csMatrix4f *m_skeletonMatrices;
  csSize m_numberOfSkeletonMatrices;

  const csUInt32 *m_skeletonBoneMapping;
  csSize m_numberOfSkeletonBoneMappings;

  csSize m_numberOfShadowMatrices;
  csMatrix4f m_shadowMatricesProjView[6];
  csMatrix4f m_shadowMatricesProj[6];
  csMatrix4f m_shadowMatricesView[6];
  csVector2f m_shadowNearFars[6];

  void InvalidateSamplers();

  /**
   * \name Render a full screen frame
   * @{
   */
  void InitFullScreenData();
  VertexBufferGL4 *m_fullScreenVertexBuffer;
  VertexBufferGL4 *m_fullScreenParamVertexBuffer;
  csVertexDeclarationGL4 *m_fullScreenVertexDeclaration;
  csProgramGL4 *m_fullScreenProgram;
  csProgramGL4 *m_fullScreenArrayProgram;
  csUInt16 m_viewportWidth;
  csUInt16 m_viewportHeight;
  csVector3f m_fadeInOutDistances;
  csUInt8 m_fadeInOutValue;
  /**
   * @}
   */

  ISGShaderGraphFactory* m_shaderGraphFactory;

  struct RenderStates
  {
    bool BlendEnabled;
    csBlendMode BlendModeSrcColor;
    csBlendMode BlendModeSrcAlpha;
    csBlendMode BlendModeDstColor;
    csBlendMode BlendModeDstAlpha;

    bool DepthMask;
    csUInt8 ColorMask;
    bool DepthTest;
    csCompareMode DepthFunc;

    csVector4f ClearColor;
    float ClearDepth;
    csUInt8 ClearStencil;

    csFaceWinding FrontFace;
    csFaceSide CullFace;
    bool CullFaceEnabled;

    csFillMode FillMode;
  };

  std::vector<RenderStates> m_renderStateStack;
};

