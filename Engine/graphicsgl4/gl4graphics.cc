
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

#define CHECK_RENDER_STATES

#ifdef CHECK_RENDER_STATES
#define RENDER_STATE_CHECK false
#else
#define RENDER_STATE_CHECK true
#endif

cs::GraphicsGL4::GraphicsGL4()
  : cs::iGraphics()
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

  for (unsigned i = 0; i < cs::eMT_COUNT; i++)
  {
    m_matrices[i].SetIdentity();
    m_matrixNeedsRecalculation[i] = false;
  }

  cs::ResourceManager::Get()->RegisterLoader(new cs::ShaderGL4Loader());
  cs::ResourceManager::Get()->RegisterLoader(new cs::ProgramGL4Loader());

  InitFullScreenData();


  ResetDefaults();

  m_shaderGraphFactory = new cs::ShaderGraphGL4(this);


}


void cs::GraphicsGL4::ResetDefaults ()
{
  m_depthMask = true;
  glDepthMask(true);

  m_colorMask = 0xf;
  glColorMask(true, true, true, true);

  m_depthTest = true;
  glEnable(GL_DEPTH_TEST);

  m_depthFunc = cs::eCM_LessOrEqual;
  glDepthFunc(GL_LEQUAL);
  CS_CHECK_GL_ERROR;

  m_clearColor = cs::Vector4f(0.0f, 0.0f, 0.0f, 0.0f);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

  m_clearDepth = 1.0f;
  glClearDepth(1.0f);

  glGetError();
  // initialize all 16 vertex buffer streams
  for (unsigned i = 0; i < 16; ++i)
  {
    SetVertexBuffer(i, 0);
  }

  for (unsigned i = 0; i < cs::eMT_COUNT; i++)
  {
    m_matrices[i].SetIdentity();
    m_matrixNeedsRecalculation[i] = false;
  }


  m_blendEnabled = false;
  m_blendModeSrcColor = cs::eBM_One;
  m_blendModeSrcAlpha = cs::eBM_One;
  m_blendModeDstColor = cs::eBM_Zero;
  m_blendModeDstAlpha = cs::eBM_Zero;
  glDisable(GL_BLEND);
  glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ZERO);

  memset(m_samplers, 0, sizeof(m_samplers));
  memset(m_textures, 0, sizeof(m_textures));
  InvalidateSamplers();
  InvalidateTextures();

  SetShader(0);

  m_frontFace = cs::eFW_CW;
  glFrontFace(GL_CW);

  m_cullFaceEnabled = true;
  glEnable(GL_CULL_FACE);

  m_cullFace = cs::eFS_Back;
  glCullFace(GL_BACK);

  m_fillMode = cs::eFM_Fill;
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  CS_CHECK_GL_ERROR;

  if (!glIsVertexArray(m_vao))
  {
    glGenVertexArrays(1, &m_vao);
  }
  glBindVertexArray(m_vao);
  CS_CHECK_GL_ERROR;
}



cs::iIndexBuffer *cs::GraphicsGL4::CreateIndexBuffer(csSize size, const void *data, cs::eBufferDataMode mode)
{
  cs::IndexBufferGL4 *indexBuffer = new cs::IndexBufferGL4();
  if (!indexBuffer->CreateBuffer(size, data, mode))
  {
    indexBuffer->Release();
    indexBuffer = 0;
  }

  return indexBuffer;
}



cs::iVertexBuffer *cs::GraphicsGL4::CreateVertexBuffer(csSize size, const void *data, cs::eBufferDataMode mode)
{
  cs::VertexBufferGL4 *vertexBuffer = new cs::VertexBufferGL4();
  if (!vertexBuffer->CreateBuffer(size, data, mode))
  {
    vertexBuffer->Release();
    vertexBuffer = 0;
  }

  return vertexBuffer;
}

cs::iVertexDeclaration *cs::GraphicsGL4::CreateVertexDeclaration(const cs::VertexElement *elements)
{
  cs::VertexDeclarationGL4 *decl = new cs::VertexDeclarationGL4();
  if (!decl->Create(elements))
  {
    decl->Release();
    decl = 0;
  }

  return decl;
}

cs::iRenderTarget *cs::GraphicsGL4::CreateRenderTarget()
{
  return new cs::RenderTargetGL4();
}

cs::iSampler *cs::GraphicsGL4::CreateSampler()
{
  cs::SamplerGL4 *sampler = new cs::SamplerGL4();
  if (!sampler->Initialize())
  {
    sampler->Release();
    sampler = 0;
  }

  return sampler;
}

cs::iTexture2D *cs::GraphicsGL4::CreateTexture2D(cs::ePixelFormat format, csUInt16 width, csUInt16 height, bool mipmaps)
{
  cs::Texture2DGL4 *texture = new cs::Texture2DGL4();
  if (!texture->Initialize(format, width, height, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}


cs::iTexture2DArray *cs::GraphicsGL4::CreateTexture2DArray(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 layers, bool mipmaps)
{
  cs::Texture2DArrayGL4 *texture = new cs::Texture2DArrayGL4();
  if (!texture->Initialize(format, width, height, layers, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}

cs::iTextureCube *cs::GraphicsGL4::CreateTextureCube(cs::ePixelFormat format, csUInt16 width, csUInt16 height, csUInt16 depth)
{
  cs::TextureCubeGL4 *texture = new cs::TextureCubeGL4();
  if (!texture->Initialize(format, width, height, depth))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}



cs::iShader *cs::GraphicsGL4::CreateShader(const std::string &vertexCode, const std::string &tessCtrlCode, const std::string &tessEvalCode, const std::string &geometryCode, const std::string &fragmentCode)
{
  cs::ProgramGL4 *program = new cs::ProgramGL4();

  cs::ShaderGL4 *vertexShader = new cs::ShaderGL4();
  vertexShader->SetShaderType(cs::eST_Vertex);
  vertexShader->SetSource(vertexCode);
  if (!vertexShader->Compile())
  {
    printf("Unable to compile vertex shader:\n%s\n", vertexShader->GetCompileErrorLog().c_str());
    CS_RELEASE(vertexShader);
    CS_RELEASE(program);
    return 0;
  }
  program->AttachShader(new cs::ShaderGL4Wrapper(vertexShader));
  CS_RELEASE(vertexShader);

  if (tessCtrlCode.length() != 0)
  {
    cs::ShaderGL4 *tessCtrlShader = new cs::ShaderGL4();
    tessCtrlShader->SetShaderType(cs::eST_TessCtrl);
    tessCtrlShader->SetSource(tessCtrlCode);
    if (!tessCtrlShader->Compile())
    {
      printf("Unable to compile tess ctrl shader:\n%s\n", tessCtrlShader->GetCompileErrorLog().c_str());
      CS_RELEASE(tessCtrlShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(new cs::ShaderGL4Wrapper(tessCtrlShader));
    CS_RELEASE(tessCtrlShader);
  }
  if (tessEvalCode.length() != 0)
  {
    cs::ShaderGL4 *tessEvalShader = new cs::ShaderGL4();
    tessEvalShader->SetShaderType(cs::eST_TessEval);
    tessEvalShader->SetSource(tessEvalCode);
    if (!tessEvalShader->Compile())
    {
      printf("Unable to compile tess eval shader:\n%s\n", tessEvalShader->GetCompileErrorLog().c_str());
      CS_RELEASE(tessEvalShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(new cs::ShaderGL4Wrapper(tessEvalShader));
    CS_RELEASE(tessEvalShader);
  }
  if (geometryCode.length() != 0)
  {
    cs::ShaderGL4 *geometryShader = new cs::ShaderGL4();
    geometryShader->SetShaderType(cs::eST_Geometry);
    geometryShader->SetSource(geometryCode);
    if (!geometryShader->Compile())
    {
      printf("Unable to compile geometry shader:\n%s\n", geometryShader->GetCompileErrorLog().c_str());
      CS_RELEASE(geometryShader);
      CS_RELEASE(program);
      return 0;
    }
    program->AttachShader(new cs::ShaderGL4Wrapper(geometryShader));
    CS_RELEASE(geometryShader);
  }
  cs::ShaderGL4 *fragmentShader = new cs::ShaderGL4();
  fragmentShader->SetShaderType(cs::eST_Fragment);
  fragmentShader->SetSource(fragmentCode);
  if (!fragmentShader->Compile())
  {
    printf("Unable to compile fragment shader:\n%s\n", fragmentShader->GetCompileErrorLog().c_str());
    CS_RELEASE(fragmentShader);
    CS_RELEASE(program);
    return 0;
  }
  program->AttachShader(new cs::ShaderGL4Wrapper(fragmentShader));
  CS_RELEASE(fragmentShader);

  if (!program->Link())
  {
    printf("Unable to link program:\n%s\n", program->GetLinkErrorLog().c_str());
    CS_RELEASE(program);
    return 0;
  }
  return program;
}

cs::iSGShaderGraphFactory *cs::GraphicsGL4::GetShaderGraphFactory()
{
  return m_shaderGraphFactory;
}



void cs::GraphicsGL4::SetProjectionMatrix(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatProj] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatProjInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModelInv] = true;
}

void cs::GraphicsGL4::SetProjectionMatrixInv(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatProjInv] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatProjInv] = false;
}

void cs::GraphicsGL4::SetViewMatrix(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatView] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatViewInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModelInv] = true;

}

void cs::GraphicsGL4::SetViewMatrixInv(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatViewInv] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatViewInv] = false;
}

void cs::GraphicsGL4::SetModelMatrix(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatModel] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatModelInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[cs::eMT_MatProjViewModelInv] = true;
}

void cs::GraphicsGL4::SetModelMatrixInv(const cs::Matrix4f &matrix)
{
  m_matrices[cs::eMT_MatModelInv] = matrix;
  m_matrixNeedsRecalculation[cs::eMT_MatModelInv] = false;
}

void cs::GraphicsGL4::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f &m)
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

void cs::GraphicsGL4::GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f &m)
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

void cs::GraphicsGL4::GetOrthographicProjection(float l, float r, float b, float t, float n, float f, cs::Matrix4f &m)
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

void cs::GraphicsGL4::GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, cs::Matrix4f &m)
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

void cs::GraphicsGL4::SetSkeleton(const cs::Skeleton *skeleton)
{
  SetSkeletonMatrices(skeleton->GetMatrices(), skeleton->GetNumberOfBones());
}

void cs::GraphicsGL4::SetSkeletonMatrices(const cs::Matrix4f *matrices, csSize numberOfMatrices)
{
  m_skeletonMatrices = matrices;
  m_numberOfSkeletonMatrices = numberOfMatrices;
}

void cs::GraphicsGL4::SetSkeletonBoneMapping(const csUInt32 *boneMapping, csSize numberOfBoneMappings)
{
  m_skeletonBoneMapping = boneMapping;
  m_numberOfSkeletonBoneMappings = numberOfBoneMappings;
}


void cs::GraphicsGL4::SetShadowMatrices(const cs::Matrix4f *projView, const cs::Matrix4f *proj, const cs::Matrix4f *view, const cs::Vector2f *nearFars, csSize numberOfMatrices)
{
  m_numberOfShadowMatrices = numberOfMatrices;
  memcpy(m_shadowMatricesProjView, projView, sizeof(cs::Matrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesProj, proj, sizeof(cs::Matrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesView, view, sizeof(cs::Matrix4f) * numberOfMatrices);
  memcpy(m_shadowNearFars, nearFars, sizeof(cs::Vector2f) * numberOfMatrices);
}

void cs::GraphicsGL4::RecalculateMatrix(cs::eMatrixType type)
{
  if (!m_matrixNeedsRecalculation[type])
  {
    return;
  }

  m_matrixNeedsRecalculation[type] = false;


  switch (type)
  {
  case cs::eMT_MatProjInv:
    m_matrices[cs::eMT_MatProj].Inverted(m_matrices[cs::eMT_MatProjInv]);
    break;
  case cs::eMT_MatViewInv:
    m_matrices[cs::eMT_MatView].Inverted(m_matrices[cs::eMT_MatViewInv]);
    break;
  case cs::eMT_MatModelInv:
    m_matrices[cs::eMT_MatModel].Inverted(m_matrices[cs::eMT_MatModelInv]);
    break;

  case cs::eMT_MatProjViewInv:
    RecalculateMatrix(cs::eMT_MatProjView);
    m_matrices[cs::eMT_MatProjView].Inverted(m_matrices[cs::eMT_MatProjViewInv]);
    break;
  case cs::eMT_MatViewModelInv:
    RecalculateMatrix(cs::eMT_MatViewModel);
    m_matrices[cs::eMT_MatViewModel].Inverted(m_matrices[cs::eMT_MatViewModelInv]);
    break;
  case cs::eMT_MatProjViewModelInv:
    RecalculateMatrix(cs::eMT_MatProjViewModel);
    m_matrices[cs::eMT_MatProjViewModel].Inverted(m_matrices[cs::eMT_MatProjViewModelInv]);
    break;

  case cs::eMT_MatProjView:
    cs::Matrix4f::Mult(m_matrices[cs::eMT_MatProj], m_matrices[cs::eMT_MatView], m_matrices[cs::eMT_MatProjView]);
    break;

  case cs::eMT_MatViewModel:
    cs::Matrix4f::Mult(m_matrices[cs::eMT_MatView], m_matrices[cs::eMT_MatModel], m_matrices[cs::eMT_MatViewModel]);
    break;

  case cs::eMT_MatProjViewModel:
    RecalculateMatrix(cs::eMT_MatProjView);
    cs::Matrix4f::Mult(m_matrices[cs::eMT_MatProjView], m_matrices[cs::eMT_MatModel], m_matrices[cs::eMT_MatProjViewModel]);
    break;

  case cs::eMT_MatProj:
  case cs::eMT_MatView:
  case cs::eMT_MatModel:
  default:
    break;
  }
}

void cs::GraphicsGL4::BindSkeleton()
{
  if (m_skeletonMatrices && m_numberOfSkeletonMatrices)
  {

    cs::iShaderAttribute *attrib = m_program->GetAttribute(cs::eVAT_MatsSkeleton);
    if (attrib)
    {
      attrib->Set(m_skeletonMatrices, m_numberOfSkeletonMatrices);
    }
  }
  if (m_skeletonBoneMapping && m_numberOfSkeletonBoneMappings)
  {
    cs::iShaderAttribute *attrib = m_program->GetAttribute(cs::eVAT_SkeletonMapping);
    if (attrib)
    {
      attrib->Set(m_skeletonBoneMapping, m_numberOfSkeletonBoneMappings);
    }
  }
}

void cs::GraphicsGL4::BindMatrices()
{
  if (!m_program)
  {
    return;
  }
  for (csUInt32 i = 0; i < cs::eMT_COUNT; ++i)
  {
    cs::iShaderAttribute *attrib = m_program->GetAttribute(cs::eVAT_MatProj + i);
    if (attrib)
    {
      RecalculateMatrix((cs::eMatrixType)i);
      attrib->Set(m_matrices[i]);
    }
  }

  
  static cs::ShaderAttributeID ShadowMatricesProjViewAttribID("ShadowMapMatProjView");
  cs::iShaderAttribute *attrib = m_program->GetAttribute(ShadowMatricesProjViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProjView, m_numberOfShadowMatrices);
  }

  static cs::ShaderAttributeID ShadowMatricesProjAttribID("ShadowMapMatProj");
  attrib = m_program->GetAttribute(ShadowMatricesProjAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProj, m_numberOfShadowMatrices);
  }

  static cs::ShaderAttributeID ShadowMatricesViewAttribID("ShadowMapMatView");
  attrib = m_program->GetAttribute(ShadowMatricesViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesView, m_numberOfShadowMatrices);
  }

  static cs::ShaderAttributeID ShadowNearFarsAttribID("ShadowMapNearFar");
  attrib = m_program->GetAttribute(ShadowNearFarsAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowNearFars, m_numberOfShadowMatrices);
  }

  CS_CHECK_GL_ERROR;
}



void cs::GraphicsGL4::SetVertexDeclaration(cs::iVertexDeclaration *vertexDeclaration)
{
  cs::VertexDeclarationGL4 *decl = static_cast<cs::VertexDeclarationGL4*>(vertexDeclaration);
  CS_SET(m_vertexDeclaration, decl);
}

void cs::GraphicsGL4::SetVertexBuffer(csUInt16 streamIdx, cs::iVertexBuffer *vertexBuffer)
{
  assert(streamIdx < 16);

  cs::VertexBufferGL4 *vb = static_cast<cs::VertexBufferGL4*>(vertexBuffer);
  CS_SET(m_vertexBuffer[streamIdx], vb);
}

void cs::GraphicsGL4::SetIndexBuffer(cs::iIndexBuffer *indexBuffer)
{
  cs::IndexBufferGL4 *ib = static_cast<cs::IndexBufferGL4*>(indexBuffer);
  CS_SET(m_indexBuffer, ib);
}


void cs::GraphicsGL4::SetShader(cs::iShader *shader)
{
  cs::ProgramGL4 *prog = static_cast<cs::ProgramGL4*>(shader);
  if (prog != m_program || RENDER_STATE_CHECK)
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

cs::eTextureUnit cs::GraphicsGL4::BindTexture(cs::iTexture *texture)
{
  if (texture == 0 || m_nextTextureUnit == cs::eTU_Invalid)
  {
    if (m_nextTextureUnit != cs::eTU_Invalid)
    {
      SetTexture(m_nextTextureUnit, 0);
    }
    return cs::eTU_Invalid;
  }

  cs::eTextureUnit unit = m_nextTextureUnit;
  m_nextTextureUnit = static_cast<cs::eTextureUnit>(m_nextTextureUnit + 1);
  glActiveTexture(GL_TEXTURE0 + m_nextTextureUnit);
  CS_CHECK_GL_ERROR;
  if (texture->GetSampler())
  {
    SetSampler(unit, texture->GetSampler()->Get());
  }
  SetTexture(unit, texture);


  return unit;
}

void cs::GraphicsGL4::SetTexture(cs::eTextureUnit unit, cs::iTexture *texture)
{
  cs::TextureGL4 *textureGL = texture ? cs::QueryClass<cs::TextureGL4>(texture) : 0;
  if (m_textures[unit] != textureGL || RENDER_STATE_CHECK)
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

void cs::GraphicsGL4::SetSampler(cs::eTextureUnit unit, cs::iSampler *sampler)
{
  cs::SamplerGL4 *samplerGL = sampler ? cs::QueryClass<cs::SamplerGL4>(sampler) : 0;
  if (m_samplers[unit] != samplerGL || RENDER_STATE_CHECK)
  {
    CS_SET(m_samplers[unit], samplerGL);
    m_samplerChanged[unit] = true;
    samplerGL->Bind(unit);
  }
}

void cs::GraphicsGL4::SetRenderTarget(cs::iRenderTarget *renderTarget)
{
  cs::RenderTargetGL4 *rtGL4 = cs::QueryClass<cs::RenderTargetGL4>(renderTarget);
  if (m_renderTarget != rtGL4 || RENDER_STATE_CHECK)
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


void cs::GraphicsGL4::PushRenderStates()
{
  RenderStates rs;
  rs.BlendEnabled = m_blendEnabled;
  rs.BlendModeDstAlpha = m_blendModeDstAlpha;
  rs.BlendModeDstColor = m_blendModeDstColor;
  rs.BlendModeSrcAlpha = m_blendModeSrcAlpha;
  rs.BlendModeSrcColor = m_blendModeSrcColor;
  rs.DepthMask = m_depthMask;
  rs.DepthTest = m_depthTest;
  rs.DepthFunc = m_depthFunc;
  rs.ColorMask = m_colorMask;

  rs.ClearColor = m_clearColor;
  rs.ClearDepth = m_clearDepth;
  rs.ClearStencil = m_clearStencil;

  rs.FrontFace = m_frontFace;
  rs.CullFace = m_cullFace;
  rs.CullFaceEnabled = m_cullFaceEnabled;

  rs.FillMode = m_fillMode;

  m_renderStateStack.push_back(rs);
}

void cs::GraphicsGL4::PopRenderStates()
{
  if (m_renderStateStack.empty())
  {
    printf("Missmatch of push and pop render states.\n");
    return;
  }

  RenderStates rs = m_renderStateStack[m_renderStateStack.size() - 1];
  m_renderStateStack.pop_back();

  SetBlendEnabled(rs.BlendEnabled);
  SetBlendMode(rs.BlendModeSrcColor, rs.BlendModeSrcAlpha, rs.BlendModeDstColor, rs.BlendModeDstAlpha);
  SetDepthMask(rs.DepthMask);

  SetColorMask(rs.ColorMask & 0x08, rs.ColorMask & 0x04, rs.ColorMask & 0x02, rs.ColorMask & 0x01);
  SetDepthTest(rs.DepthTest);
  SetDepthFunc(rs.DepthFunc);

  SetClearColorValue(rs.ClearColor);
  SetClearDepthValue(rs.ClearDepth);
  SetClearStencilValue(rs.ClearStencil);

  SetFrontFace(rs.FrontFace);
  SetCullFace(rs.CullFace);
  SetCullFaceEnabled(rs.CullFaceEnabled);

  SetFillMode(rs.FillMode);
}


void cs::GraphicsGL4::SetBlendEnabled(bool enable)
{
  if (m_blendEnabled != enable || RENDER_STATE_CHECK)
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

bool cs::GraphicsGL4::IsBlendEnabled() const
{
  return m_blendEnabled;
}

void cs::GraphicsGL4::SetBlendMode(cs::eBlendMode blendSrc, cs::eBlendMode blendDst)
{
  if (blendSrc != m_blendModeSrcColor || blendSrc != m_blendModeSrcAlpha || blendDst != m_blendModeDstColor || blendDst != m_blendModeDstAlpha || RENDER_STATE_CHECK)
  {
    m_blendModeSrcColor = m_blendModeSrcAlpha = blendSrc;
    m_blendModeDstColor = m_blendModeDstAlpha = blendDst;
    glBlendFunc(blendModeMap[blendSrc], blendModeMap[blendDst]);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::SetBlendMode(cs::eBlendMode blendSrcColor, cs::eBlendMode blendSrcAlpha, cs::eBlendMode blendDstColor, cs::eBlendMode blendDstAlpha)
{
  if (blendSrcColor != m_blendModeSrcColor || blendSrcAlpha != m_blendModeSrcAlpha || blendDstColor != m_blendModeDstColor || blendDstAlpha != m_blendModeDstAlpha || RENDER_STATE_CHECK)
  {
    m_blendModeSrcColor = blendSrcColor;
    m_blendModeSrcAlpha = blendSrcAlpha;
    m_blendModeDstColor = blendDstColor;
    m_blendModeDstAlpha = blendDstAlpha;
    glBlendFuncSeparate(blendModeMap[blendSrcColor], blendModeMap[blendSrcAlpha], blendModeMap[blendDstColor], blendModeMap[blendDstAlpha]);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::GetBlendMode(cs::eBlendMode &blendSrcColor, cs::eBlendMode &blendDstColor, cs::eBlendMode &blendSrcAlpha, cs::eBlendMode &blendDstAlpha) const
{
  blendSrcColor = m_blendModeSrcColor;
  blendSrcAlpha = m_blendModeSrcAlpha;
  blendDstColor = m_blendModeDstColor;
  blendDstAlpha = m_blendModeDstAlpha;
}


void cs::GraphicsGL4::SetDepthMask(bool depth)
{
  if (m_depthMask != depth || RENDER_STATE_CHECK)
  {
    m_depthMask = depth;
    glDepthMask(m_depthMask);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::SetColorMask(bool red, bool green, bool blue, bool alpha)
{
  csUInt8 colorMask = (red ? 0x8 : 0x0) | (green ? 0x4 : 0x0) | (blue ? 0x2 : 0x0) | (alpha ? 0x1 : 0x0);
  if (colorMask != m_colorMask || RENDER_STATE_CHECK)
  {
    m_colorMask = colorMask;
    glColorMask(red, green, blue, alpha);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::SetDepthTest(bool depthTest)
{
  if (m_depthTest != depthTest || RENDER_STATE_CHECK)
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

void cs::GraphicsGL4::SetDepthFunc(cs::eCompareMode compareMode)
{
  if (m_depthFunc != compareMode || RENDER_STATE_CHECK)
  {
    m_depthFunc = compareMode;
    glDepthFunc(compareFuncMap[compareMode]);
    CS_CHECK_GL_ERROR;
  }
}


void cs::GraphicsGL4::SetFrontFace(cs::eFaceWinding frontFace)
{
  if (m_frontFace != frontFace || RENDER_STATE_CHECK)
  {
    m_frontFace = frontFace;
    glFrontFace(faceWindingMap[frontFace]);
  }
}

cs::eFaceWinding cs::GraphicsGL4::GetFrontFace() const
{
  return m_frontFace;
}

void cs::GraphicsGL4::SetCullFaceEnabled(bool enabled)
{
  if (m_cullFaceEnabled != enabled || RENDER_STATE_CHECK)
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

bool cs::GraphicsGL4::IsCullFaceEnabled() const
{
  return m_cullFaceEnabled;
}

void cs::GraphicsGL4::SetCullFace(cs::eFaceSide cullFace)
{
  if (m_cullFace != cullFace || RENDER_STATE_CHECK)
  {
    m_cullFace = cullFace;
    glCullFace(faceSideMap[cullFace]);
  }
}

cs::eFaceSide cs::GraphicsGL4::GetCullFace() const
{
  return m_cullFace;
}

void cs::GraphicsGL4::SetFillMode(cs::eFillMode fillMode)
{
  if (m_fillMode != fillMode || RENDER_STATE_CHECK)
  {
    glPolygonMode(GL_FRONT_AND_BACK, fillModeMap[fillMode]);
    m_fillMode = fillMode;
  }
}

cs::eFillMode cs::GraphicsGL4::GetFillMode() const
{
  return m_fillMode;
}

void cs::GraphicsGL4::SetRenderFadeInOut(float near, float far)
{
  m_fadeInOutDistances.Set(near, far, far - near);
}

void cs::GraphicsGL4::SetRenderFadeInOutValue(csUInt8 value)
{
  m_fadeInOutValue = value;
}

void cs::GraphicsGL4::SetRenderDestination(cs::eRenderDestination renderDestination)
{
  glDrawBuffer(renderDestinationMap[renderDestination]);
  CS_CHECK_GL_ERROR;
}

void cs::GraphicsGL4::SetRenderDestinations(cs::eRenderDestination *renderDestination, csSize numRenderDestinations)
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

void cs::GraphicsGL4::SetClearColorValue(const cs::Vector4f &colorValue)
{
  if (colorValue != m_clearColor || RENDER_STATE_CHECK)
  {
    m_clearColor = colorValue;
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::SetClearDepthValue(float depthValue)
{
  if (depthValue != m_clearDepth || RENDER_STATE_CHECK)
  {
    m_clearDepth = depthValue;
    glClearDepth(m_clearDepth);
    CS_CHECK_GL_ERROR;
  }
}

void cs::GraphicsGL4::SetClearStencilValue(csUInt8 stencilValue)
{
  if (stencilValue != m_clearStencil || RENDER_STATE_CHECK)
  {
    m_clearStencil = stencilValue;
    glClearStencil(m_clearStencil);
    CS_CHECK_GL_ERROR;
  }
}


void cs::GraphicsGL4::Clear(bool clearColor, const cs::Vector4f &color, bool clearDepth, float depth, bool clearStencil, csUInt8 stencil)
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

void cs::GraphicsGL4::SetViewport(csUInt16 width, csUInt16 height)
{
  SetViewport(0, 0, width, height);
}

void cs::GraphicsGL4::SetViewport(csInt16 x, csInt16 y, csUInt16 width, csUInt16 height)
{
  glViewport(x, y, width, height);
  m_viewportWidth = width;
  m_viewportHeight = height;
  CS_CHECK_GL_ERROR;
}

void cs::GraphicsGL4::SetViewport(cs::iRenderTarget *renderTarget)
{
  SetViewport(0, 0, renderTarget->GetWidth(), renderTarget->GetHeight());
}


void cs::GraphicsGL4::BindValues()
{
  BindMatrices();



  /* ******************************************* */
  /*    Bind data for the fading gradient        */
  /* ******************************************* */
  cs::iShaderAttribute *attribBinaryGradient = m_program->GetAttribute(cs::eVAT_BinaryGradient);
  if (attribBinaryGradient)
  {
    cs::eTextureUnit tu = BindTexture(cs::BinaryGradient::GetBinaryGradient()->Get());
    if (tu != cs::eTU_Invalid)
    {
      attribBinaryGradient->Set(tu);
    }
  }

  cs::iShaderAttribute *attributeBinaryGradientToScreen = m_program->GetAttribute(cs::eVAT_BinaryGradientToScreen);
  if (attributeBinaryGradientToScreen)
  {
    attributeBinaryGradientToScreen->Set((float)m_viewportWidth / 8.0f, (float)m_viewportHeight / 8.0f);
  }

  cs::iShaderAttribute *attributeFadeInOutValue = m_program->GetAttribute(cs::eVAT_FadeInOutValue);
  if (attributeFadeInOutValue)
  {
    attributeFadeInOutValue->Set(m_fadeInOutValue);
  }
  cs::iShaderAttribute *attributeFadeInOutDist = m_program->GetAttribute(cs::eVAT_FadeInOutDist);
  if (attributeFadeInOutDist)
  {
    attributeFadeInOutDist->Set(m_fadeInOutDistances);
  }

  /* ******************************************* */
  /*    Bind other data                          */
  /* ******************************************* */
  cs::iShaderAttribute *attributeViewportSize = m_program->GetAttribute(cs::eVAT_ViewportSize);
  if (attributeViewportSize)
  {
    attributeViewportSize->Set((float)m_viewportWidth, (float)m_viewportHeight);
  }

  cs::iShaderAttribute *attributeViewportSizeInv = m_program->GetAttribute(cs::eVAT_ViewportSizeInv);
  if (attributeViewportSizeInv)
  {
    attributeViewportSizeInv->Set(1.0f / (float)m_viewportWidth, 1.0f / (float)m_viewportHeight);
  }

}


void cs::GraphicsGL4::Render(cs::ePrimitiveType primType, csUInt32 count)
{
  BindValues();
  if (BindVertexDeclaration())
  {
    glDrawArrays(primitiveTypeMap[primType], 0, count);
    CS_CHECK_GL_ERROR;

    UnbindVertexDeclaration();
  }
}

void cs::GraphicsGL4::RenderIndexed(cs::ePrimitiveType primType, csUInt32 count, cs::eDataType indexType)
{
  csInt32 c = count;
  if (c < 0)
  {
    return;
  }
  BindValues();
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    CS_CHECK_GL_ERROR;
    glDrawElements(primitiveTypeMap[primType], count, dataTypeMap[indexType], 0);
    CS_CHECK_GL_ERROR;

    UnbindVertexDeclaration();
  }
}

void cs::GraphicsGL4::RenderFullScreenFrame(cs::iTexture2D *texture)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture);
}

void cs::GraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, cs::iTexture2D *texture)
{
  static cs::ShaderAttributeID attrDiffuseID("Diffuse");
  static cs::ShaderAttributeID attrLeftBottomID("LeftBottom");
  static cs::ShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  SetDepthTest(true);
  //SetDepthMask(true);
  SetDepthFunc(cs::eCM_Always);
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
  cs::eTextureUnit tu = BindTexture(texture);
  if (m_fullScreenProgram->GetAttribute(attrDiffuseID))
  {
    m_fullScreenProgram->GetAttribute(attrDiffuseID)->Set(tu);
  }
  m_fullScreenProgram->GetAttribute(attrLeftBottomID)->Set(cs::Vector2f(x0, y0));
  m_fullScreenProgram->GetAttribute(attrDeltaID)->Set(cs::Vector2f(x1 - x0, y1 - y0));

  Render(cs::ePT_Triangles, 6);
}

void cs::GraphicsGL4::RenderFullScreenFrame(cs::iTexture2DArray *texture, int layer)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture, layer);
}

void cs::GraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, cs::iTexture2DArray *texture, int layer)
{
  static cs::ShaderAttributeID attrDiffuseID("Diffuse");
  static cs::ShaderAttributeID attrLayerID("Layer");
  static cs::ShaderAttributeID attrLeftBottomID("LeftBottom");
  static cs::ShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  SetDepthMask(true);
  SetDepthTest(true);
  SetDepthFunc(cs::eCM_Always);
  SetDepthMask(false);
  glClearDepth(1.0);
  CS_CHECK_GL_ERROR;

  SetShader(m_fullScreenArrayProgram);
  SetVertexBuffer(0, m_fullScreenParamVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  cs::eTextureUnit tu = BindTexture(texture);
  m_fullScreenArrayProgram->GetAttribute(attrDiffuseID)->Set(tu);
  m_fullScreenArrayProgram->GetAttribute(attrLeftBottomID)->Set(cs::Vector2f(x0, y0));
  m_fullScreenArrayProgram->GetAttribute(attrDeltaID)->Set(cs::Vector2f(x1 - x0, y1 - y0));
  m_fullScreenArrayProgram->GetAttribute(attrLayerID)->Set(layer);
  Render(cs::ePT_Triangles, 6);
}


void cs::GraphicsGL4::RenderFullScreenFrame()
{
  SetDepthMask(true);
  SetDepthTest(false);
  SetDepthFunc(cs::eCM_Always);
  SetDepthMask(false);
  //glClearDepth(1.0);

  SetVertexBuffer(0, m_fullScreenVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  Render(cs::ePT_Triangles, 6);
}


bool cs::GraphicsGL4::BindVertexDeclaration()
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

void cs::GraphicsGL4::UnbindVertexDeclaration()
{
  cs::VertexBufferGL4::Unbind();
  for (unsigned i = 0, in = m_vertexDeclaration->GetNumberOfStreams(); i < in; ++i)
  {
    m_vertexDeclaration->UnbindStream(m_program, i);
  }

}


void cs::GraphicsGL4::FreeTextures()
{
  for (int i = 0; i < cs::eTU_COUNT; ++i)
  {
    SetTexture((cs::eTextureUnit)i, 0);
  }
}

void cs::GraphicsGL4::InvalidateSamplers()
{
  for (int i = 0; i < cs::eTU_COUNT; ++i)
  {
    m_samplerChanged[i] = true;
  }
}


void cs::GraphicsGL4::InvalidateTextures()
{
  m_nextTextureUnit = cs::eTU_TextureUnit0;
  for (int i = 0; i < cs::eTU_COUNT; ++i)
  {
    m_textureChanged[i] = true;
  }
}


void cs::GraphicsGL4::InitFullScreenData()
{
  float vertexDataParam[] = {
    0, 0, 0, 1,     0, 0,
    0, 1, 0, 1,     0, 1,
    1, 1, 0, 1,     1, 1,

    0, 0, 0, 1,     0, 0,
    1, 1, 0, 1,     1, 1,
    1, 0, 0, 1,     1, 0,
  };
  m_fullScreenParamVertexBuffer = static_cast<cs::VertexBufferGL4*>(CreateVertexBuffer(sizeof(vertexDataParam), vertexDataParam, cs::eBDM_Static));

  float vertexData[] = {
    -1, -1, 0, 1,     0, 0,
    -1, 1, 0, 1,     0, 1,
     1, 1, 0, 1,     1, 1,

    -1, -1, 0, 1,     0, 0,
     1, 1, 0, 1,     1, 1,
     1, -1, 0, 1,     1, 0,
  };
  m_fullScreenVertexBuffer = static_cast<cs::VertexBufferGL4*>(CreateVertexBuffer(sizeof(vertexData), vertexData, cs::eBDM_Static));

  cs::VertexElement elements[] = {
    cs::VertexElement(cs::eVST_Position, cs::eDT_Float, 4, 0, 6 * sizeof(float), 0),
    cs::VertexElement(cs::eVST_TexCoord0, cs::eDT_Float, 2, 4 * sizeof(float), 6 * sizeof(float), 0),
    cs::VertexElement(),
  };

  m_fullScreenVertexDeclaration = static_cast<cs::VertexDeclarationGL4*>(CreateVertexDeclaration(elements));

  m_fullScreenProgram = cs::ResourceManager::Get()->GetOrLoad<cs::ProgramGL4>(cs::ResourceLocator("${shaders}/renderer/SimplePresent.asset"));
  m_fullScreenArrayProgram = cs::ResourceManager::Get()->GetOrLoad<cs::ProgramGL4>(cs::ResourceLocator("${shaders}/renderer/ArrayPresent.asset "));
}


void cs::GraphicsGL4::AssetGraphicsErrors() const
{
  CS_CHECK_GL_ERROR;
}
