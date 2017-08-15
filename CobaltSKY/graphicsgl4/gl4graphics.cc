
#include <graphicsgl4/gl4graphics.hh>
#include <graphicsgl4/gl4rendertarget.hh>
#include <graphicsgl4/gl4indexbuffer.hh>
#include <graphicsgl4/gl4program.hh>
#include <graphicsgl4/gl4programloader.hh>
#include <graphicsgl4/gl4sampler.hh>
#include <graphicsgl4/gl4shader.hh>
#include <graphicsgl4/gl4shaderattribute.hh>
#include <graphicsgl4/gl4shaderstream.hh>
#include <graphicsgl4/gl4shaderloader.hh>
#include <graphicsgl4/gl4texture.hh>
#include <graphicsgl4/gl4texture2d.hh>
#include <graphicsgl4/gl4texture2darray.hh>
#include <graphicsgl4/gl4texturecube.hh>
#include <graphicsgl4/gl4mapping.hh>
#include <graphicsgl4/gl4defines.hh>
#include <graphicsgl4/gl4vertexbuffer.hh>
#include <graphicsgl4/gl4vertexdeclaration.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <cobalt/animation/csskeleton.hh>
#include <cobalt/graphics/csbinarygradient.hh>
#include <GL/glew.h>
#include <assert.h>


csGraphicsGL4::csGraphicsGL4()
  : iGraphics()
  , m_vertexDeclaration(0)
  , m_indexBuffer(0)
  , m_program(0)
  , m_renderTarget(0)
  , m_skeletonMatrices(0)
  , m_numberOfSkeletonMatrices(0)
  , m_skeletonBoneMapping(0)
  , m_numberOfSkeletonBoneMappings(0)
  , m_shaderGraphFactory(0)
{
  CS_CLASS_GEN_CONSTR;
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    printf("Initialize GLEW failed\n");
  }
  //
  // it appears glewInit ends up with an invalid enum pending in the error state
  // so we clear that state here so we can start up clean
  glGetError();
  
  printf("OpenGL Stats:\n");
  printf("  Vendor  : %s\n", (const char *)glGetString(GL_VENDOR));
  printf("  Renderer: %s\n", (const char *)glGetString(GL_RENDERER));
  printf("  Version : %s\n", (const char *)glGetString(GL_VERSION));
  printf("  GLSL    : %s\n", (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION));

  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  glPixelStorei(GL_PACK_ALIGNMENT, 4);
  // initialize all 16 vertex buffer streams
  for (unsigned i = 0; i < 16; ++i)
  {
    m_vertexBuffer[i] = 0;
  }

  for (unsigned i = 0; i < eMT_COUNT; i++)
  {
    m_matrices[i].SetIdentity();
    m_matrixNeedsRecalculation[i] = false;
  }

  csResourceManager::Get()->RegisterLoader(new csShaderGL4Loader());
  csResourceManager::Get()->RegisterLoader(new csProgramGL4Loader());

  InitFullScreenData();


  ResetDefaults();

  m_shaderGraphFactory = new csShaderGraphGL4(this);


}


void csGraphicsGL4::ResetDefaults ()
{
  m_depthMask = true;
  glDepthMask(true);

  m_colorMask = 0xf;
  glColorMask(true, true, true, true);

  m_depthTest = true;
  glEnable(GL_DEPTH_TEST);

  m_depthFunc = eCM_LessOrEqual;
  glDepthFunc(GL_LEQUAL);
  CS_CHECK_GL_ERROR;

  SetClearColorValue(csVector4f(0.0f, 0.0f, 0.0f, 0.0f));
  SetClearDepthValue(1.0f);

  glGetError();
  // initialize all 16 vertex buffer streams
  for (unsigned i = 0; i < 16; ++i)
  {
    SetVertexBuffer(i, 0);
  }

  for (unsigned i = 0; i < eMT_COUNT; i++)
  {
    m_matrices[i].SetIdentity();
    m_matrixNeedsRecalculation[i] = false;
  }

  m_blendEnabled = false;
  m_blendModeSrcColor = m_blendModeSrcAlpha = eBM_One;
  m_blendModeDstColor = m_blendModeDstAlpha = eBM_Zero;

  memset(m_samplers, 0, sizeof(m_samplers));
  memset(m_textures, 0, sizeof(m_textures));
  InvalidateSamplers();
  InvalidateTextures();

  SetShader(0);

  m_frontFace = eFW_CW;
  glFrontFace(GL_CW);

  m_cullFaceEnabled = true;
  glEnable(GL_CULL_FACE);

  m_cullFace = eFS_Back;
  glCullFace(GL_BACK);
  CS_CHECK_GL_ERROR;

  if (!glIsVertexArray(m_vao))
  {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    CS_CHECK_GL_ERROR;
  }
}



iIndexBuffer *csGraphicsGL4::CreateIndexBuffer(csSize size, const void *data, csBufferDataMode mode)
{
  IndexBufferGL4 *indexBuffer = new IndexBufferGL4();
  if (!indexBuffer->CreateBuffer(size, data, mode))
  {
    indexBuffer->Release();
    indexBuffer = 0;
  }

  return indexBuffer;
}



iVertexBuffer *csGraphicsGL4::CreateVertexBuffer(csSize size, const void *data, csBufferDataMode mode)
{
  VertexBufferGL4 *vertexBuffer = new VertexBufferGL4();
  if (!vertexBuffer->CreateBuffer(size, data, mode))
  {
    vertexBuffer->Release();
    vertexBuffer = 0;
  }

  return vertexBuffer;
}

iVertexDeclaration *csGraphicsGL4::CreateVertexDeclaration(const csVertexElement *elements)
{
  csVertexDeclarationGL4 *decl = new csVertexDeclarationGL4();
  if (!decl->Create(elements))
  {
    decl->Release();
    decl = 0;
  }

  return decl;
}

iRenderTarget *csGraphicsGL4::CreateRenderTarget()
{
  return new csRenderTargetGL4();
}

iSampler *csGraphicsGL4::CreateSampler()
{
  csSamplerGL4 *sampler = new csSamplerGL4();
  if (!sampler->Initialize())
  {
    sampler->Release();
    sampler = 0;
  }

  return sampler;
}

iTexture2D *csGraphicsGL4::CreateTexture2D(csPixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps)
{
  csTexture2DGL4 *texture = new csTexture2DGL4();
  if (!texture->Initialize(format, width, height, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}


iTexture2DArray *csGraphicsGL4::CreateTexture2DArray(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps)
{
  csTexture2DArrayGL4 *texture = new csTexture2DArrayGL4();
  if (!texture->Initialize(format, width, height, layers, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}

iTextureCube *csGraphicsGL4::CreateTextureCube(csPixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth)
{
  csTextureCubeGL4 *texture = new csTextureCubeGL4();
  if (!texture->Initialize(format, width, height, depth))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}



iShader *csGraphicsGL4::CreateShader(const csString &vertexCode, const csString &tessCtrlCode, const csString &tessEvalCode, const csString &geometryCode, const csString &fragmentCode)
{
  csProgramGL4 *program = new csProgramGL4();

  csShaderGL4 *vertexShader = new csShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexCode);
  if (!vertexShader->Compile())
  {
    printf("Unable to compile vertex shader:\n%s\n", vertexShader->GetCompileErrorLog().c_str());
    CS_RELEASE(vertexShader);
    CS_RELEASE(program);
    return 0;
  }
  program->AttachShader(vertexShader);
  CS_RELEASE(vertexShader);

  if (tessCtrlCode.length() != 0)
  {
    csShaderGL4 *tessCtrlShader = new csShaderGL4();
    tessCtrlShader->SetShaderType(eST_TessCtrl);
    tessCtrlShader->SetSource(tessCtrlCode);
    if (!tessCtrlShader->Compile())
    {
      printf("Unable to compile tess ctrl shader:\n%s\n", tessCtrlShader->GetCompileErrorLog().c_str());
      CS_RELEASE(tessCtrlShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(tessCtrlShader);
    CS_RELEASE(tessCtrlShader);
  }
  if (tessEvalCode.length() != 0)
  {
    csShaderGL4 *tessEvalShader = new csShaderGL4();
    tessEvalShader->SetShaderType(eST_TessEval);
    tessEvalShader->SetSource(tessEvalCode);
    if (!tessEvalShader->Compile())
    {
      printf("Unable to compile tess eval shader:\n%s\n", tessEvalShader->GetCompileErrorLog().c_str());
      CS_RELEASE(tessEvalShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(tessEvalShader);
    CS_RELEASE(tessEvalShader);
  }
  if (geometryCode.length() != 0)
  {
    csShaderGL4 *geometryShader = new csShaderGL4();
    geometryShader->SetShaderType(eST_Geometry);
    geometryShader->SetSource(geometryCode);
    if (!geometryShader->Compile())
    {
      printf("Unable to compile geometry shader:\n%s\n", geometryShader->GetCompileErrorLog().c_str());
      CS_RELEASE(geometryShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(geometryShader);
    CS_RELEASE(geometryShader);
  }
  csShaderGL4 *fragmentShader = new csShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentCode);
  if (!fragmentShader->Compile())
  {
    printf("Unable to compile fragment shader:\n%s\n", fragmentShader->GetCompileErrorLog().c_str());
    CS_RELEASE(fragmentShader);
    CS_RELEASE(program);
    return 0;
  }
  program->AttachShader(fragmentShader);
  CS_RELEASE(fragmentShader);

  if (!program->Link())
  {
    printf("Unable to link program:\n%s\n", program->GetLinkErrorLog().c_str());
    CS_RELEASE(program);
    return 0;
  }
  return program;
}

ISGShaderGraphFactory *csGraphicsGL4::GetShaderGraphFactory()
{
  return m_shaderGraphFactory;
}


void csGraphicsGL4::SetProjectionMatrix(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatProj] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void csGraphicsGL4::SetProjectionMatrixInv(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatProjInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = false;
}

void csGraphicsGL4::SetViewMatrix(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatView] = matrix;
  m_matrixNeedsRecalculation[eMT_MatViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;

}

void csGraphicsGL4::SetViewMatrixInv(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatViewInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatViewInv] = false;
}

void csGraphicsGL4::SetModelMatrix(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatModel] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void csGraphicsGL4::SetModelMatrixInv(const csMatrix4f &matrix)
{
  m_matrices[eMT_MatModelInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = false;
}

void csGraphicsGL4::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;

  /*
  m.m00 = z2 / dx; m.m10 = 0.0f;    m.m20 = sx / dx;  m.m30 = 0.0f;
  m.m01 = 0.0f;    m.m11 = z2 / dy; m.m21 = sy / dy;  m.m31 = 0.0f;
  m.m02 = 0.0f;    m.m12 = 0.0f;    m.m22 = -sz / dz; m.m32 = -2.0f*n*f / dz;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = -1.0f;    m.m33 = 0.0f;
  */
  m.m00 = z2 / dx; m.m10 = sx / dx; m.m20 = 0.0f;    m.m30 = 0.0f;
  m.m01 = 0.0f;    m.m11 = sy / dy; m.m21 = z2 / dy; m.m31 = 0.0f;
  m.m02 = 0.0f;    m.m12 = sz / dz; m.m22 = 0.0f;    m.m32 = -2.0f*n*f / dz;
  m.m03 = 0.0f;    m.m13 = 1.0f;    m.m23 = 0.0f;    m.m33 = 0.0f;

}

void csGraphicsGL4::GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &m)
{
  float z2 = 2.0f * n;
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;
  float nf2 = z2 * f;

  /*
  m.m00 = dx / z2; m.m10 = 0.0f;    m.m20 = 0.0f;      m.m30 = sx / z2;
  m.m01 = 0.0f;    m.m11 = dy / z2; m.m21 = 0.0f;      m.m31 = sy / z2;
  m.m02 = 0.0f;    m.m12 = 0.0f;    m.m22 = 0.0f;      m.m32 = -1.0f;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = -dz / nf2; m.m33 = sz / nf2;
  */

  m.m00 = dx / z2; m.m10 = 0.0f;    m.m20 = 0.0f;      m.m30 = sx / z2;
  m.m01 = 0.0f;    m.m11 = 0.0f;    m.m21 = 0.0f;      m.m31 = 1.0f;
  m.m02 = 0.0f;    m.m12 = dy / z2; m.m22 = 0.0f;      m.m32 = sy / z2;
  m.m03 = 0.0f;    m.m13 = 0.0f;    m.m23 = -dz / nf2; m.m33 = sz / nf2;

}

void csGraphicsGL4::GetOrthographicProjection(float l, float r, float b, float t, float n, float f, csMatrix4f &m)
{
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = f + n;

  /*
  m.m00 = 2.0f / dx; m.m10 = 0.0f;      m.m20 = 0.0f;      m.m30 = -sx / dx;
  m.m01 = 0.0f;      m.m11 = 2.0f / dy; m.m21 = 0.0f;      m.m31 = -sy / dy;
  m.m02 = 0.0f;      m.m12 = 0.0f;      m.m22 = 2.0f / dz; m.m32 = -sz / dz;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;      m.m33 = 1.0;
  */

  m.m00 = 2.0f / dx; m.m10 = 0.0f;      m.m20 = 0.0f;      m.m30 = -sx / dx;
  m.m01 = 0.0f;      m.m11 = 0.0f;      m.m21 = 2.0f / dy; m.m31 = -sy / dy;
  m.m02 = 0.0f;      m.m12 = 2.0f / dz; m.m22 = 0.0f;      m.m32 = -sz / dz;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;      m.m33 = 1.0;
}

void csGraphicsGL4::GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, csMatrix4f &m)
{
  float dx = r - l;
  float dy = t - b;
  float dz = f - n;
  float sx = r + l;
  float sy = t + b;
  float sz = n + f;

  /*
  m.m00 = dx / 2.0f; m.m10 = 0.0f;      m.m20 = 0.0f;       m.m30 = sx / 2.0f;
  m.m01 = 0.0f;      m.m11 = dy / 2.0f; m.m21 = 0.0f;       m.m31 = sy / 2.0f;
  m.m02 = 0.0f;      m.m12 = 0.0f;      m.m22 = dz / 2.0f;  m.m32 = sz / 2.0f;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;       m.m33 = 1.0;
  */

  m.m00 = dx / 2.0f; m.m10 = 0.0f;      m.m20 = 0.0f;       m.m30 = sx / 2.0f;
  m.m01 = 0.0f;      m.m11 = 0.0f;      m.m21 = dz / 2.0f;  m.m31 = sz / 2.0f;
  m.m02 = 0.0f;      m.m12 = dy / 2.0f; m.m22 = 0.0f;       m.m32 = sy / 2.0f;
  m.m03 = 0.0f;      m.m13 = 0.0f;      m.m23 = 0.0f;       m.m33 = 1.0;

}

void csGraphicsGL4::SetSkeleton(const csSkeleton *skeleton)
{
  SetSkeletonMatrices(skeleton->GetMatrices(), skeleton->GetNumberOfBones());
}

void csGraphicsGL4::SetSkeletonMatrices(const csMatrix4f *matrices, csSize numberOfMatrices)
{
  m_skeletonMatrices = matrices;
  m_numberOfSkeletonMatrices = numberOfMatrices;
}

void csGraphicsGL4::SetSkeletonBoneMapping(const csUInt32 *boneMapping, csSize numberOfBoneMappings)
{
  m_skeletonBoneMapping = boneMapping;
  m_numberOfSkeletonBoneMappings = numberOfBoneMappings;
}


void csGraphicsGL4::SetShadowMatrices(const csMatrix4f *projView, const csMatrix4f *proj, const csMatrix4f *view, const csVector2f *nearFars, csSize numberOfMatrices)
{
  m_numberOfShadowMatrices = numberOfMatrices;
  memcpy(m_shadowMatricesProjView, projView, sizeof(csMatrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesProj, proj, sizeof(csMatrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesView, view, sizeof(csMatrix4f) * numberOfMatrices);
  memcpy(m_shadowNearFars, nearFars, sizeof(csVector2f) * numberOfMatrices);
}

void csGraphicsGL4::RecalculateMatrix(csMatrixType type)
{
  if (!m_matrixNeedsRecalculation[type])
  {
    return;
  }

  m_matrixNeedsRecalculation[type] = false;


  switch (type)
  {
  case eMT_MatProjInv:
    m_matrices[eMT_MatProj].Inverted(m_matrices[eMT_MatProjInv]);
    break;
  case eMT_MatViewInv:
    m_matrices[eMT_MatView].Inverted(m_matrices[eMT_MatViewInv]);
    break;
  case eMT_MatModelInv:
    m_matrices[eMT_MatModel].Inverted(m_matrices[eMT_MatModelInv]);
    break;

  case eMT_MatProjViewInv:
    RecalculateMatrix(eMT_MatProjView);
    m_matrices[eMT_MatProjView].Inverted(m_matrices[eMT_MatProjViewInv]);
    break;
  case eMT_MatViewModelInv:
    RecalculateMatrix(eMT_MatViewModel);
    m_matrices[eMT_MatViewModel].Inverted(m_matrices[eMT_MatViewModelInv]);
    break;
  case eMT_MatProjViewModelInv:
    RecalculateMatrix(eMT_MatProjViewModel);
    m_matrices[eMT_MatProjViewModel].Inverted(m_matrices[eMT_MatProjViewModelInv]);
    break;

  case eMT_MatProjView:
    csMatrix4f::Mult(m_matrices[eMT_MatProj], m_matrices[eMT_MatView], m_matrices[eMT_MatProjView]);
    break;

  case eMT_MatViewModel:
    csMatrix4f::Mult(m_matrices[eMT_MatView], m_matrices[eMT_MatModel], m_matrices[eMT_MatViewModel]);
    break;

  case eMT_MatProjViewModel:
    RecalculateMatrix(eMT_MatProjView);
    csMatrix4f::Mult(m_matrices[eMT_MatProjView], m_matrices[eMT_MatModel], m_matrices[eMT_MatProjViewModel]);
    break;

  case eMT_MatProj:
  case eMT_MatView:
  case eMT_MatModel:
  default:
    break;
  }
}

void csGraphicsGL4::BindSkeleton()
{
  if (m_skeletonMatrices && m_numberOfSkeletonMatrices)
  {

    iShaderAttribute *attrib = m_program->GetAttribute(eVAT_MatsSkeleton);
    if (attrib)
    {
      attrib->Set(m_skeletonMatrices, m_numberOfSkeletonMatrices);
    }
  }
  if (m_skeletonBoneMapping && m_numberOfSkeletonBoneMappings)
  {
    iShaderAttribute *attrib = m_program->GetAttribute(eVAT_SkeletonMapping);
    if (attrib)
    {
      attrib->Set(m_skeletonBoneMapping, m_numberOfSkeletonBoneMappings);
    }
  }
}

void csGraphicsGL4::BindMatrices()
{
  if (!m_program)
  {
    return;
  }
  for (csUInt32 i = 0; i < eMT_COUNT; ++i)
  {
    iShaderAttribute *attrib = m_program->GetAttribute(eVAT_MatProj + i);
    if (attrib)
    {
      RecalculateMatrix((csMatrixType)i);
      attrib->Set(m_matrices[i]);
    }
  }

  
  static csShaderAttributeID ShadowMatricesProjViewAttribID("ShadowMapMatProjView");
  iShaderAttribute *attrib = m_program->GetAttribute(ShadowMatricesProjViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProjView, m_numberOfShadowMatrices);
  }

  static csShaderAttributeID ShadowMatricesProjAttribID("ShadowMapMatProj");
  attrib = m_program->GetAttribute(ShadowMatricesProjAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProj, m_numberOfShadowMatrices);
  }

  static csShaderAttributeID ShadowMatricesViewAttribID("ShadowMapMatView");
  attrib = m_program->GetAttribute(ShadowMatricesViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesView, m_numberOfShadowMatrices);
  }

  static csShaderAttributeID ShadowNearFarsAttribID("ShadowMapNearFar");
  attrib = m_program->GetAttribute(ShadowNearFarsAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowNearFars, m_numberOfShadowMatrices);
  }

  CS_CHECK_GL_ERROR;
}


void csGraphicsGL4::SetVertexDeclaration(iVertexDeclaration *vertexDeclaration)
{
  csVertexDeclarationGL4 *decl = static_cast<csVertexDeclarationGL4*>(vertexDeclaration);
  CS_SET(m_vertexDeclaration, decl);
}

void csGraphicsGL4::SetVertexBuffer(csUInt16 streamIdx, iVertexBuffer *vertexBuffer)
{
  assert(streamIdx < 16);

  VertexBufferGL4 *vb = static_cast<VertexBufferGL4*>(vertexBuffer);
  CS_SET(m_vertexBuffer[streamIdx], vb);
}

void csGraphicsGL4::SetIndexBuffer(iIndexBuffer *indexBuffer)
{
  IndexBufferGL4 *ib = static_cast<IndexBufferGL4*>(indexBuffer);
  CS_SET(m_indexBuffer, ib);
}


void csGraphicsGL4::SetShader(iShader *shader)
{
  csProgramGL4 *prog = static_cast<csProgramGL4*>(shader);
  if (prog != m_program)
  {
    CS_SET(m_program, prog);

    if (prog)
    {
      // we use the new program
      prog->Bind();
    }
    else
    {
      glUseProgram(0);
      CS_CHECK_GL_ERROR;
    }
  }
  InvalidateTextures();
}

csTextureUnit csGraphicsGL4::BindTexture(iTexture *texture)
{
  if (texture == 0 || m_nextTextureUnit == eTU_Invalid)
  {
    if (m_nextTextureUnit != eTU_Invalid)
    {
      SetTexture(m_nextTextureUnit, 0);
    }
    return eTU_Invalid;
  }

  csTextureUnit unit = m_nextTextureUnit;
  m_nextTextureUnit = static_cast<csTextureUnit>(m_nextTextureUnit + 1);
  glActiveTexture(GL_TEXTURE0 + m_nextTextureUnit);
  CS_CHECK_GL_ERROR;
  SetSampler(unit, texture->GetSampler());
  SetTexture(unit, texture);


  return unit;
}

void csGraphicsGL4::SetTexture(csTextureUnit unit, iTexture *texture)
{
  csTextureGL4 *textureGL = texture ? csQueryClass<csTextureGL4>(texture) : 0;
  if (m_textures[unit] != textureGL)
  {
    CS_SET(m_textures[unit], textureGL);
    m_textureChanged[unit] = true;
    if (texture)
    {
      glActiveTexture(GL_TEXTURE0 + unit);
      CS_CHECK_GL_ERROR;
      textureGL->Bind();
    }
  }
}

void csGraphicsGL4::SetSampler(csTextureUnit unit, iSampler *sampler)
{
  csSamplerGL4 *samplerGL = sampler ? csQueryClass<csSamplerGL4>(sampler) : 0;
  if (m_samplers[unit] != samplerGL)
  {
    CS_SET(m_samplers[unit], samplerGL);
    m_samplerChanged[unit] = true;
    samplerGL->Bind(unit);
  }
}

void csGraphicsGL4::SetRenderTarget(iRenderTarget *renderTarget)
{
  csRenderTargetGL4 *rtGL4 = csQueryClass<csRenderTargetGL4>(renderTarget);
  if (m_renderTarget != rtGL4 )
  {
    CS_SET(m_renderTarget, rtGL4);
    if (m_renderTarget)
    {
      m_renderTarget->Bind();
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      CS_CHECK_GL_ERROR;
    }
  }
}


void csGraphicsGL4::SetBlendEnabled(bool enable)
{
  if (m_blendEnabled != enable || true)
  {
    m_blendEnabled = enable;
    if (enable)
    {
      glEnable(GL_BLEND);
    }
    else
    {
      glDisable(GL_BLEND);
    }
    CS_CHECK_GL_ERROR;
  }
}

bool csGraphicsGL4::IsBlendEnabled() const
{
  return m_blendEnabled;
}

void csGraphicsGL4::SetBlendMode(csBlendMode blendSrc, csBlendMode blendDst)
{
  if (blendSrc != m_blendModeSrcColor || blendSrc != m_blendModeSrcAlpha || blendDst != m_blendModeDstColor || blendDst != m_blendModeDstAlpha)
  {
    m_blendModeSrcColor = m_blendModeSrcAlpha = blendSrc;
    m_blendModeDstColor = m_blendModeDstAlpha = blendDst;
    glBlendFunc(blendModeMap[blendSrc], blendModeMap[blendDst]);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetBlendMode(csBlendMode blendSrcColor, csBlendMode blendSrcAlpha, csBlendMode blendDstColor, csBlendMode blendDstAlpha)
{
  if (blendSrcColor != m_blendModeSrcColor || blendSrcAlpha != m_blendModeSrcAlpha || blendDstColor != m_blendModeDstColor || blendDstAlpha != m_blendModeDstAlpha)
  {
    m_blendModeSrcColor = blendSrcColor;
    m_blendModeSrcAlpha = blendSrcAlpha;
    m_blendModeDstColor = blendDstColor;
    m_blendModeDstAlpha = blendDstAlpha;
    glBlendFuncSeparate(blendModeMap[blendSrcColor], blendModeMap[blendSrcAlpha], blendModeMap[blendDstColor], blendModeMap[blendDstAlpha]);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::GetBlendMode(csBlendMode &blendSrcColor, csBlendMode &blendDstColor, csBlendMode &blendSrcAlpha, csBlendMode &blendDstAlpha) const
{
  blendSrcColor = m_blendModeSrcColor;
  blendSrcAlpha = m_blendModeSrcAlpha;
  blendDstColor = m_blendModeDstColor;
  blendDstAlpha = m_blendModeDstAlpha;
}


void csGraphicsGL4::SetDepthMask(bool depth)
{
  if (m_depthMask != depth)
  {
    m_depthMask = depth;
    glDepthMask(m_depthMask);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetColorMask(bool red, bool green, bool blue, bool alpha)
{
  csUInt8 colorMask = (red ? 0x8 : 0x0) | (green ? 0x4 : 0x0) | (blue ? 0x2 : 0x0) | (alpha ? 0x1 : 0x0);
  if (colorMask != m_colorMask)
  {
    m_colorMask = colorMask;
    glColorMask(red, green, blue, alpha);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetDepthTest(bool depthTest)
{
  if (m_depthTest != depthTest)
  {
    m_depthTest = depthTest;
    if (m_depthTest)
    {
      glEnable(GL_DEPTH_TEST);
    }
    else
    {
      glDisable(GL_DEPTH_TEST);
    }
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetDepthFunc(csCompareMode compareMode)
{
  if (m_depthFunc != compareMode)
  {
    m_depthFunc = compareMode;
    glDepthFunc(compareFuncMap[compareMode]);
    CS_CHECK_GL_ERROR;
  }
}


void csGraphicsGL4::SetFrontFace(csFaceWinding frontFace)
{
  if (m_frontFace != frontFace)
  {
    m_frontFace = frontFace;
    glFrontFace(faceWindingMap[frontFace]);
  }
}

csFaceWinding csGraphicsGL4::GetFrontFace() const
{
  return m_frontFace;
}

void csGraphicsGL4::SetCullFaceEnabled(bool enabled)
{
  if (m_cullFaceEnabled != enabled)
  {
    m_cullFaceEnabled = enabled;
    if (enabled)
    {
      glEnable(GL_CULL_FACE);
    }
    else
    {
      glDisable(GL_CULL_FACE);
    }
  }
}

bool csGraphicsGL4::IsCullFaceEnabled() const
{
  return m_cullFaceEnabled;
}

void csGraphicsGL4::SetCullFace(csFaceSide cullFace)
{
  if (m_cullFace != cullFace)
  {
    m_cullFace = cullFace;
    glCullFace(faceSideMap[cullFace]);
  }
}

csFaceSide csGraphicsGL4::GetCullFace() const
{
  return m_cullFace;
}


void csGraphicsGL4::SetRenderFadeInOut(float near, float far)
{
  m_fadeInOutDistances.Set(near, far, far - near);
}

void csGraphicsGL4::SetRenderFadeInOutValue(csUInt8 value)
{
  m_fadeInOutValue = value;
}

void csGraphicsGL4::SetRenderDestination(csRenderDestination renderDestination)
{
  glDrawBuffer(renderDestinationMap[renderDestination]);
  CS_CHECK_GL_ERROR;
}

void csGraphicsGL4::SetRenderDestinations(csRenderDestination *renderDestination, csSize numRenderDestinations)
{
  static GLenum destinations[16];
  if (numRenderDestinations > 16)
  {
    numRenderDestinations = 16;
  }
  
  for (csSize i = 0; i < numRenderDestinations; ++i)
  {
    destinations[i] = renderDestinationMap[renderDestination[i]];
  }
  glDrawBuffers(numRenderDestinations, destinations);
  CS_CHECK_GL_ERROR;
}

void csGraphicsGL4::SetClearColorValue(const csVector4f &colorValue)
{
  if (colorValue != m_clearColor)
  {
    m_clearColor = colorValue;
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetClearDepthValue(float depthValue)
{
  if (depthValue != m_clearDepth)
  {
    m_clearDepth = depthValue;
    glClearDepth(m_clearDepth);
    CS_CHECK_GL_ERROR;
  }
}

void csGraphicsGL4::SetClearStencilValue(csUInt8 stencilValue)
{
  if (stencilValue != m_clearStencil)
  {
    m_clearStencil = stencilValue;
    glClearStencil(m_clearStencil);
    CS_CHECK_GL_ERROR;
  }
}


void csGraphicsGL4::Clear(bool clearColor, const csVector4f &color, bool clearDepth, float depth, bool clearStencil, csUInt8 stencil)
{
  GLbitfield clear = 0;
  if (clearColor)
  {
    clear |= GL_COLOR_BUFFER_BIT;
    SetClearColorValue(color);
  }

  if (clearDepth)
  {
    clear |= GL_DEPTH_BUFFER_BIT;
    SetClearDepthValue(clearDepth);
  }

  if (clearStencil)
  {
    clear |= GL_STENCIL_BUFFER_BIT;
    SetClearStencilValue(stencil);
  }

  if (clear == 0)
  {
    return;
  }

  glClear(clear);
  CS_CHECK_GL_ERROR;

}

void csGraphicsGL4::SetViewport(csUInt16 width, csUInt16 height)
{
  SetViewport(0, 0, width, height);
}

void csGraphicsGL4::SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height)
{
  glViewport(x, y, width, height);
  m_viewportWidth = width;
  m_viewportHeight = height;
  CS_CHECK_GL_ERROR;
}

void csGraphicsGL4::SetViewport(iRenderTarget *renderTarget)
{
  SetViewport(0, 0, renderTarget->GetWidth(), renderTarget->GetHeight());
}


void csGraphicsGL4::BindValues()
{
  BindMatrices();



  /* ******************************************* */
  /*    Bind data for the fading gradient        */
  /* ******************************************* */
  iShaderAttribute *attribBinaryGradient = m_program->GetAttribute(eVAT_BinaryGradient);
  if (attribBinaryGradient)
  {
    csTextureUnit tu = BindTexture(csBinaryGradient::GetBinaryGradient());
    if (tu != eTU_Invalid)
    {
      attribBinaryGradient->Set(tu);
    }
  }

  iShaderAttribute *attributeBinaryGradientToScreen = m_program->GetAttribute(eVAT_BinaryGradientToScreen);
  if (attributeBinaryGradientToScreen)
  {
    attributeBinaryGradientToScreen->Set((float)m_viewportWidth / 8.0f, (float)m_viewportHeight / 8.0f);
  }

  iShaderAttribute *attributeFadeInOutValue = m_program->GetAttribute(eVAT_FadeInOutValue);
  if (attributeFadeInOutValue)
  {
    attributeFadeInOutValue->Set(m_fadeInOutValue);
  }
  iShaderAttribute *attributeFadeInOutDist = m_program->GetAttribute(eVAT_FadeInOutDist);
  if (attributeFadeInOutDist)
  {
    attributeFadeInOutDist->Set(m_fadeInOutDistances);
  }

  /* ******************************************* */
  /*    Bind other data                          */
  /* ******************************************* */
  iShaderAttribute *attributeViewportSize = m_program->GetAttribute(eVAT_ViewportSize);
  if (attributeViewportSize)
  {
    attributeViewportSize->Set((float)m_viewportWidth, (float)m_viewportHeight);
  }

  iShaderAttribute *attributeViewportSizeInv = m_program->GetAttribute(eVAT_ViewportSizeInv);
  if (attributeViewportSizeInv)
  {
    attributeViewportSizeInv->Set(1.0f / (float)m_viewportWidth, 1.0f / (float)m_viewportHeight);
  }

}


void csGraphicsGL4::Render(csPrimitiveType primType, csUInt32 count)
{
  BindValues();
  if (BindVertexDeclaration())
  {
    glDrawArrays(primitiveTypeMap[primType], 0, count);
    CS_CHECK_GL_ERROR;

    UnbindVertexDeclaration();
  }
}

void csGraphicsGL4::RenderIndexed(csPrimitiveType primType, csUInt32 count, csDataType indexType)
{
  BindValues();
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    glDrawElements(primitiveTypeMap[primType], count, dataTypeMap[indexType], 0);
    CS_CHECK_GL_ERROR;

    UnbindVertexDeclaration();
  }
}

void csGraphicsGL4::RenderFullScreenFrame(iTexture2D *texture)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture);
}

void csGraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, iTexture2D *texture)
{
  static csShaderAttributeID attrDiffuseID("Diffuse");
  static csShaderAttributeID attrLeftBottomID("LeftBottom");
  static csShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  SetDepthTest(true);
  //SetDepthMask(true);
  SetDepthFunc(eCM_Always);
  SetClearDepthValue(1.0f);
  SetBlendEnabled(false);
  
  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_ALWAYS);
  //glClearDepth(1.0);
  //glDisable(GL_BLEND);
  CS_CHECK_GL_ERROR;

  SetShader(m_fullScreenProgram);
  SetVertexBuffer(0, m_fullScreenParamVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  csTextureUnit tu = BindTexture(texture);
  if (m_fullScreenProgram->GetAttribute(attrDiffuseID))
  {
    m_fullScreenProgram->GetAttribute(attrDiffuseID)->Set(tu);
  }
  m_fullScreenProgram->GetAttribute(attrLeftBottomID)->Set(csVector2f(x0, y0));
  m_fullScreenProgram->GetAttribute(attrDeltaID)->Set(csVector2f(x1 - x0, y1 - y0));

  Render(ePT_Triangles, 6);
}

void csGraphicsGL4::RenderFullScreenFrame(iTexture2DArray *texture, int layer)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture, layer);
}

void csGraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, iTexture2DArray *texture, int layer)
{
  static csShaderAttributeID attrDiffuseID("Diffuse");
  static csShaderAttributeID attrLayerID("Layer");
  static csShaderAttributeID attrLeftBottomID("LeftBottom");
  static csShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  SetDepthMask(true);
  SetDepthTest(true);
  SetDepthFunc(eCM_Always);
  SetDepthMask(false);
  glClearDepth(1.0);
  CS_CHECK_GL_ERROR;

  SetShader(m_fullScreenArrayProgram);
  SetVertexBuffer(0, m_fullScreenParamVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  csTextureUnit tu = BindTexture(texture);
  m_fullScreenArrayProgram->GetAttribute(attrDiffuseID)->Set(tu);
  m_fullScreenArrayProgram->GetAttribute(attrLeftBottomID)->Set(csVector2f(x0, y0));
  m_fullScreenArrayProgram->GetAttribute(attrDeltaID)->Set(csVector2f(x1 - x0, y1 - y0));
  m_fullScreenArrayProgram->GetAttribute(attrLayerID)->Set(layer);
  Render(ePT_Triangles, 6);
}


void csGraphicsGL4::RenderFullScreenFrame()
{
  SetDepthMask(true);
  SetDepthTest(false);
  SetDepthFunc(eCM_Always);
  SetDepthMask(false);
  //glClearDepth(1.0);

  SetVertexBuffer(0, m_fullScreenVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  Render(ePT_Triangles, 6);
}


bool csGraphicsGL4::BindVertexDeclaration()
{
  if (!(m_vertexDeclaration && m_program))
  {
    return false;
  }
  for (unsigned i = 0, in = m_vertexDeclaration->GetNumberOfStreams(); i < in; ++i)
  {
    if (!m_vertexBuffer[i])
    {
      return false;
    }

    m_vertexBuffer[i]->Bind();
    m_vertexDeclaration->BindStream(m_program, i, 0);
  }

  return true;
}

void csGraphicsGL4::UnbindVertexDeclaration()
{
  VertexBufferGL4::Unbind();
  for (unsigned i = 0, in = m_vertexDeclaration->GetNumberOfStreams(); i < in; ++i)
  {
    m_vertexDeclaration->UnbindStream(m_program, i);
  }

}


void csGraphicsGL4::FreeTextures()
{
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    SetTexture((csTextureUnit)i, 0);
  }
}

void csGraphicsGL4::InvalidateSamplers()
{
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_samplerChanged[i] = true;
  }
}


void csGraphicsGL4::InvalidateTextures()
{
  m_nextTextureUnit = eTU_TextureUnit0;
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_textureChanged[i] = true;
  }
}


void csGraphicsGL4::InitFullScreenData()
{
  float vertexDataParam[] = {
    0, 0, 0, 1,     0, 0,
    0, 1, 0, 1,     0, 1,
    1, 1, 0, 1,     1, 1,

    0, 0, 0, 1,     0, 0,
    1, 1, 0, 1,     1, 1,
    1, 0, 0, 1,     1, 0,
  };
  m_fullScreenParamVertexBuffer = static_cast<VertexBufferGL4*>(CreateVertexBuffer(sizeof(vertexDataParam), vertexDataParam, eBDM_Static));

  float vertexData[] = {
    -1, -1, 0, 1,     0, 0,
    -1, 1, 0, 1,     0, 1,
     1, 1, 0, 1,     1, 1,

    -1, -1, 0, 1,     0, 0,
     1, 1, 0, 1,     1, 1,
     1, -1, 0, 1,     1, 0,
  };
  m_fullScreenVertexBuffer = static_cast<VertexBufferGL4*>(CreateVertexBuffer(sizeof(vertexData), vertexData, eBDM_Static));

  csVertexElement elements[] = {
    csVertexElement(eVST_Position, eDT_Float, 4, 0, 6 * sizeof(float), 0),
    csVertexElement(eVST_TexCoord0, eDT_Float, 2, 4 * sizeof(float), 6 * sizeof(float), 0),
    csVertexElement(),
  };

  m_fullScreenVertexDeclaration = static_cast<csVertexDeclarationGL4*>(CreateVertexDeclaration(elements));

  m_fullScreenProgram = csResourceManager::Get()->GetOrLoad<csProgramGL4>(csResourceLocator("${shaders}/renderer/SimplePresent.xasset"));
  m_fullScreenArrayProgram = csResourceManager::Get()->GetOrLoad<csProgramGL4>(csResourceLocator("${shaders}/renderer/ArrayPresent.xasset "));
}


void csGraphicsGL4::AssetGraphicsErrors() const
{
  CS_CHECK_GL_ERROR;
}