
#pragma once

#include <graphicsgl4/gl4export.hh>
#include <cobalt/graphics/igraphics.hh>
#include <graphicsgl4/gl4graphics.refl.hh>

class csTextureGL4;
class VertexBufferGL4;
class csVertexDeclarationGL4;

namespace cs
{

class IndexBufferGL4;
class ProgramGL4;
class RenderTargetGL4;
class SamplerGL4;

CS_CLASS()
class CSGRAPHICSGL4_API GraphicsGL4 : public CS_SUPER(cs::iGraphics)
{
  CS_CLASS_GEN_OBJECT;
public:
  GraphicsGL4();
  virtual void ResetDefaults();

  virtual cs::iIndexBuffer* CreateIndexBuffer(csSize size, const void* data, cs::eBufferDataMode mode);
  virtual cs::iVertexBuffer* CreateVertexBuffer(csSize size, const void* data, cs::eBufferDataMode mode);
  virtual cs::iVertexDeclaration* CreateVertexDeclaration(const cs::VertexElement * elements);
  virtual cs::iRenderTarget* CreateRenderTarget();
  virtual cs::iSampler* CreateSampler();
  virtual cs::iTexture2D* CreateTexture2D(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps);
  virtual cs::iTexture2DArray* CreateTexture2DArray(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps);
  virtual cs::iTextureCube* CreateTextureCube(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth);
  virtual cs::iShader* CreateShader(const std::string & vertexCode, const std::string & tessCtrlCode, const std::string & tessEvalCode, const std::string & geometryCode, const std::string & fragmentCode);

  virtual cs::iSGShaderGraphFactory* GetShaderGraphFactory();

  virtual void SetProjectionMatrix(const cs::Matrix4f & matrix);
  virtual void SetProjectionMatrixInv(const cs::Matrix4f & matrix);
  virtual void SetViewMatrix(const cs::Matrix4f & matrix);
  virtual void SetViewMatrixInv(const cs::Matrix4f & matrix);
  virtual void SetModelMatrix(const cs::Matrix4f & matrix);
  virtual void SetModelMatrixInv(const cs::Matrix4f & matrix);
  virtual void GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f & out);
  virtual void GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f & out);
  virtual void GetOrthographicProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f & out);
  virtual void GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f & out);
  virtual void SetSkeleton(const cs::Skeleton * skeleton);
  virtual void SetSkeletonMatrices(const cs::Matrix4f * matrices, csSize numberOfMatrices);
  virtual void SetSkeletonBoneMapping(const csUInt32 * mapping, csSize numberOfBoneMappings);

  virtual void SetShadowMatrices(const cs::Matrix4f * projView, const cs::Matrix4f * proj, const cs::Matrix4f * view, const cs::Vector2f * nearFars, csSize numberOfMatrices);

  virtual void SetVertexDeclaration(cs::iVertexDeclaration * vertexDeclaration);
  virtual void SetVertexBuffer(csUInt16 streamIdx, cs::iVertexBuffer * vertexBuffer);
  virtual void SetIndexBuffer(cs::iIndexBuffer * indexBuffer);
  virtual void SetShader(cs::iShader * shader);
  virtual void FreeTextures();
  virtual void InvalidateTextures();
  virtual cs::eTextureUnit BindTexture(cs::iTexture * texture);
  virtual void SetTexture(cs::eTextureUnit unit, cs::iTexture * texture);
  virtual void SetSampler(cs::eTextureUnit unit, cs::iSampler * sampler);
  virtual void SetRenderTarget(cs::iRenderTarget * renderTarget);

  virtual void PushRenderStates();
  virtual void PopRenderStates();
  virtual void SetBlendEnabled(bool enabled);
  virtual bool IsBlendEnabled() const;
  virtual void SetBlendMode(cs::eBlendMode blendSrc, cs::eBlendMode blendDst);
  virtual void SetBlendMode(cs::eBlendMode blendSrcColor, cs::eBlendMode blendDstColor, cs::eBlendMode blendSrcAlpha, cs::eBlendMode blendDstAlpha);
  virtual void GetBlendMode(cs::eBlendMode & blendSrcColor, cs::eBlendMode & blendDstColor, cs::eBlendMode & blendSrcAlpha, cs::eBlendMode & blendDstAlpha) const;

  virtual void SetDepthMask(bool depth);
  virtual void SetColorMask(bool red, bool green, bool blue, bool alpha);
  virtual void SetDepthTest(bool depthTest);
  virtual void SetDepthFunc(cs::eCompareMode compareMode);

  virtual void SetFrontFace(cs::eFaceWinding faceWinding);
  virtual cs::eFaceWinding GetFrontFace() const;
  virtual void SetCullFaceEnabled(bool enable);
  virtual bool IsCullFaceEnabled() const;
  virtual void SetCullFace(cs::eFaceSide side);
  virtual cs::eFaceSide GetCullFace() const;

  virtual void SetFillMode(cs::eFillMode fillMode);
  virtual cs::eFillMode GetFillMode() const;

  virtual void SetRenderFadeInOut(float near, float far);
  virtual void SetRenderFadeInOutValue(csUInt8 value);

  virtual void SetRenderDestination(cs::eRenderDestination renderDestination);
  virtual void SetRenderDestinations(cs::eRenderDestination * renderDestination, csSize numRenderDestinations);

  virtual void Clear(bool clearColor = true, const cs::Vector4f & color = cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f), bool clearDepth = true, float depth = 1.0, bool clearStencil = false, csUInt8 stencil = 0);
  virtual void SetViewport(csUInt16 width, csUInt16 height);
  virtual void SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height);
  virtual void SetViewport(cs::iRenderTarget * viewport);
  virtual void Render(cs::ePrimitiveType type, csUInt32 count);
  virtual void RenderIndexed(cs::ePrimitiveType type, csUInt32 count, cs::eDataType indexDataType);
  virtual void BindValues();

  virtual void RenderFullScreenFrame();
  virtual void RenderFullScreenFrame(cs::iTexture2D * texture);
  virtual void RenderFullScreenFrame(float left, float right, float bottom, float top, cs::iTexture2D * texture);

  virtual void RenderFullScreenFrame(cs::iTexture2DArray * texture, int layer);
  virtual void RenderFullScreenFrame(float left, float right, float bottom, float top, cs::iTexture2DArray * texture, int layer);

  void AssetGraphicsErrors() const;

private:
  void BindMatrices();
  void BindSkeleton();
  bool BindVertexDeclaration();
  void UnbindVertexDeclaration();

  void SetClearColorValue(const cs::Vector4f & clearColorValue);
  void SetClearDepthValue(float clearDepthValue);
  void SetClearStencilValue(csUInt8 clearStencilValue);

private:
  void RecalculateMatrix(cs::eMatrixType type);

  csUInt32 m_vao;

  csVertexDeclarationGL4 * m_vertexDeclaration;
  cs::IndexBufferGL4 * m_indexBuffer;
  VertexBufferGL4 * m_vertexBuffer[16];
  cs::ProgramGL4 * m_program;
  cs::RenderTargetGL4 * m_renderTarget;

  bool m_blendEnabled;
  cs::eBlendMode m_blendModeSrcColor;
  cs::eBlendMode m_blendModeSrcAlpha;
  cs::eBlendMode m_blendModeDstColor;
  cs::eBlendMode m_blendModeDstAlpha;

  bool m_depthMask;
  csUInt8 m_colorMask;
  bool m_depthTest;
  cs::eCompareMode m_depthFunc;

  cs::Vector4f m_clearColor;
  float m_clearDepth;
  csUInt8 m_clearStencil;

  cs::eFaceWinding m_frontFace;
  cs::eFaceSide m_cullFace;
  bool m_cullFaceEnabled;

  cs::eFillMode m_fillMode;

  cs::eTextureUnit m_nextTextureUnit;

  cs::SamplerGL4 * m_samplers[cs::eTU_COUNT];
  bool m_samplerChanged[cs::eTU_COUNT];

  csTextureGL4 * m_textures[cs::eTU_COUNT];
  bool m_textureChanged[cs::eTU_COUNT];

  cs::Matrix4f m_matrices[cs::eMT_COUNT];
  bool m_matrixNeedsRecalculation[cs::eMT_COUNT];

  const cs::Matrix4f * m_skeletonMatrices;
  csSize m_numberOfSkeletonMatrices;

  const csUInt32 * m_skeletonBoneMapping;
  csSize m_numberOfSkeletonBoneMappings;

  csSize m_numberOfShadowMatrices;
  cs::Matrix4f m_shadowMatricesProjView[6];
  cs::Matrix4f m_shadowMatricesProj[6];
  cs::Matrix4f m_shadowMatricesView[6];
  cs::Vector2f m_shadowNearFars[6];

  void InvalidateSamplers();

  /**
   * \name Render a full screen frame
   * @{
   */
  void InitFullScreenData();
  VertexBufferGL4 * m_fullScreenVertexBuffer;
  VertexBufferGL4 * m_fullScreenParamVertexBuffer;
  csVertexDeclarationGL4 * m_fullScreenVertexDeclaration;
  cs::ProgramGL4 * m_fullScreenProgram;
  cs::ProgramGL4 * m_fullScreenArrayProgram;
  csUInt16 m_viewportWidth;
  csUInt16 m_viewportHeight;
  cs::Vector3f m_fadeInOutDistances;
  csUInt8 m_fadeInOutValue;
  /**
   * @}
   */

  cs::iSGShaderGraphFactory * m_shaderGraphFactory;

  struct RenderStates
  {
    bool BlendEnabled;
    cs::eBlendMode BlendModeSrcColor;
    cs::eBlendMode BlendModeSrcAlpha;
    cs::eBlendMode BlendModeDstColor;
    cs::eBlendMode BlendModeDstAlpha;

    bool DepthMask;
    csUInt8 ColorMask;
    bool DepthTest;
    cs::eCompareMode DepthFunc;

    cs::Vector4f ClearColor;
    float ClearDepth;
    csUInt8 ClearStencil;

    cs::eFaceWinding FrontFace;
    cs::eFaceSide CullFace;
    bool CullFaceEnabled;

    cs::eFillMode FillMode;
  };

  std::vector<RenderStates> m_renderStateStack;
};

}

