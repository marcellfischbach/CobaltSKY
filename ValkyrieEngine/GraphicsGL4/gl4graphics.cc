
#include <GraphicsGL4/gl4graphics.hh>
#include <GraphicsGL4/gl4rendertarget.hh>
#include <GraphicsGL4/gl4indexbuffer.hh>
#include <GraphicsGL4/gl4program.hh>
#include <GraphicsGL4/gl4programloader.hh>
#include <GraphicsGL4/gl4sampler.hh>
#include <GraphicsGL4/gl4shader.hh>
#include <GraphicsGL4/gl4shaderattribute.hh>
#include <GraphicsGL4/gl4shaderstream.hh>
#include <GraphicsGL4/gl4shaderloader.hh>
#include <GraphicsGL4/gl4texture.hh>
#include <GraphicsGL4/gl4texture2d.hh>
#include <GraphicsGL4/gl4texture2darray.hh>
#include <GraphicsGL4/gl4texturecube.hh>
#include <GraphicsGL4/gl4mapping.hh>
#include <GraphicsGL4/gl4defines.hh>
#include <GraphicsGL4/gl4vertexbuffer.hh>
#include <GraphicsGL4/gl4vertexdeclaration.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraph.hh>
#include <valkyrie/animation/vkskeleton.hh>
#include <valkyrie/graphics/vkbinarygradient.hh>
#include <GL/glew.h>
#include <assert.h>


vkGraphicsGL4::vkGraphicsGL4()
  : IGraphics()
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
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    printf("Initialize GLEW failed\n");
  }

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

  vkResourceManager::Get()->RegisterLoader(new vkShaderGL4Loader());
  vkResourceManager::Get()->RegisterLoader(new vkProgramGL4Loader());

  InitFullScreenData();

  ResetDefaults();

  m_shaderGraphFactory = new vkShaderGraphGL4(this);


  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


}


void vkGraphicsGL4::ResetDefaults ()
{
  VK_CLASS_GEN_CONSTR;
  VK_CHECK_GL_ERROR;
  m_depthMask = true;
  glDepthMask(true);

  m_colorMask = 0xf;
  glColorMask(true, true, true, true);

  m_depthTest = true;
  glEnable(GL_DEPTH_TEST);

  m_depthFunc = eCM_LessOrEqual;
  glDepthFunc(GL_LEQUAL);


  glClearDepth(1.0);
  VK_CHECK_GL_ERROR;

  glGetError();
  VK_CHECK_GL_ERROR;
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



  glFrontFace(GL_CW);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);


}



IIndexBuffer *vkGraphicsGL4::CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode)
{
  IndexBufferGL4 *indexBuffer = new IndexBufferGL4();
  if (!indexBuffer->CreateBuffer(size, data, mode))
  {
    indexBuffer->Release();
    indexBuffer = 0;
  }

  return indexBuffer;
}



IVertexBuffer *vkGraphicsGL4::CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode)
{
  VertexBufferGL4 *vertexBuffer = new VertexBufferGL4();
  if (!vertexBuffer->CreateBuffer(size, data, mode))
  {
    vertexBuffer->Release();
    vertexBuffer = 0;
  }

  return vertexBuffer;
}

IVertexDeclaration *vkGraphicsGL4::CreateVertexDeclaration(const vkVertexElement *elements)
{
  vkVertexDeclarationGL4 *decl = new vkVertexDeclarationGL4();
  if (!decl->Create(elements))
  {
    decl->Release();
    decl = 0;
  }

  return decl;
}

IRenderTarget *vkGraphicsGL4::CreateRenderTarget()
{
  return new vkRenderTargetGL4();
}

ISampler *vkGraphicsGL4::CreateSampler()
{
  vkSamplerGL4 *sampler = new vkSamplerGL4();
  if (!sampler->Initialize())
  {
    sampler->Release();
    sampler = 0;
  }

  return sampler;
}

ITexture2D *vkGraphicsGL4::CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height, bool mipmaps)
{
  vkTexture2DGL4 *texture = new vkTexture2DGL4();
  if (!texture->Initialize(format, width, height, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}


ITexture2DArray *vkGraphicsGL4::CreateTexture2DArray(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 layers, bool mipmaps)
{
  vkTexture2DArrayGL4 *texture = new vkTexture2DArrayGL4();
  if (!texture->Initialize(format, width, height, layers, mipmaps))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}

ITextureCube *vkGraphicsGL4::CreateTextureCube(vkPixelFormat format, vkUInt16 width, vkUInt16 height, vkUInt16 depth)
{
  vkTextureCubeGL4 *texture = new vkTextureCubeGL4();
  if (!texture->Initialize(format, width, height, depth))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}



IShader *vkGraphicsGL4::CreateShader(const vkString &vertexCode, const vkString &tessCtrlCode, const vkString &tessEvalCode, const vkString &geometryCode, const vkString &fragmentCode)
{
  vkProgramGL4 *program = new vkProgramGL4();

  vkShaderGL4 *vertexShader = new vkShaderGL4();
  vertexShader->SetShaderType(eST_Vertex);
  vertexShader->SetSource(vertexCode);
  if (!vertexShader->Compile())
  {
    printf("Unable to compile vertex shader:\n%s\n", vertexShader->GetCompileErrorLog().c_str());
    VK_RELEASE(vertexShader);
    VK_RELEASE(program);
    return 0;
  }
  program->AttachShader(vertexShader);
  VK_RELEASE(vertexShader);

  if (tessCtrlCode.length() != 0)
  {
    vkShaderGL4 *tessCtrlShader = new vkShaderGL4();
    tessCtrlShader->SetShaderType(eST_TessCtrl);
    tessCtrlShader->SetSource(tessCtrlCode);
    if (!tessCtrlShader->Compile())
    {
      printf("Unable to compile tess ctrl shader:\n%s\n", tessCtrlShader->GetCompileErrorLog().c_str());
      VK_RELEASE(tessCtrlShader);
      VK_RELEASE(program);
      return 0;
    }
    program->AttachShader(tessCtrlShader);
    VK_RELEASE(tessCtrlShader);
  }
  if (tessEvalCode.length() != 0)
  {
    vkShaderGL4 *tessEvalShader = new vkShaderGL4();
    tessEvalShader->SetShaderType(eST_TessEval);
    tessEvalShader->SetSource(tessEvalCode);
    if (!tessEvalShader->Compile())
    {
      printf("Unable to compile tess eval shader:\n%s\n", tessEvalShader->GetCompileErrorLog().c_str());
      VK_RELEASE(tessEvalShader);
      VK_RELEASE(program);
      return 0;
    }
    program->AttachShader(tessEvalShader);
    VK_RELEASE(tessEvalShader);
  }
  if (geometryCode.length() != 0)
  {
    vkShaderGL4 *geometryShader = new vkShaderGL4();
    geometryShader->SetShaderType(eST_Geometry);
    geometryShader->SetSource(geometryCode);
    if (!geometryShader->Compile())
    {
      printf("Unable to compile geometry shader:\n%s\n", geometryShader->GetCompileErrorLog().c_str());
      VK_RELEASE(geometryShader);
      VK_RELEASE(program);
      return 0;
    }
    program->AttachShader(geometryShader);
    VK_RELEASE(geometryShader);
  }
  vkShaderGL4 *fragmentShader = new vkShaderGL4();
  fragmentShader->SetShaderType(eST_Fragment);
  fragmentShader->SetSource(fragmentCode);
  if (!fragmentShader->Compile())
  {
    printf("Unable to compile fragment shader:\n%s\n", fragmentShader->GetCompileErrorLog().c_str());
    VK_RELEASE(fragmentShader);
    VK_RELEASE(program);
    return 0;
  }
  program->AttachShader(fragmentShader);
  VK_RELEASE(fragmentShader);

  if (!program->Link())
  {
    printf("Unable to link program:\n%s\n", program->GetLinkErrorLog().c_str());
    VK_RELEASE(program);
    return 0;
  }
  return program;
}

ISGShaderGraphFactory *vkGraphicsGL4::GetShaderGraphFactory()
{
  return m_shaderGraphFactory;
}


void vkGraphicsGL4::SetProjectionMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatProj] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void vkGraphicsGL4::SetProjectionMatrixInv(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatProjInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = false;
}

void vkGraphicsGL4::SetViewMatrix(const vkMatrix4f &matrix)
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

void vkGraphicsGL4::SetViewMatrixInv(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatViewInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatViewInv] = false;
}

void vkGraphicsGL4::SetModelMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatModel] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void vkGraphicsGL4::SetModelMatrixInv(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatModelInv] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = false;
}

void vkGraphicsGL4::GetPerspectiveProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &m)
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

void vkGraphicsGL4::GetPerspectiveProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &m)
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

void vkGraphicsGL4::GetOrthographicProjection(float l, float r, float b, float t, float n, float f, vkMatrix4f &m)
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

void vkGraphicsGL4::GetOrthographicProjectionInv(float l, float r, float b, float t, float n, float f, vkMatrix4f &m)
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

void vkGraphicsGL4::SetSkeleton(const vkSkeleton *skeleton)
{
  SetSkeletonMatrices(skeleton->GetMatrices(), skeleton->GetNumberOfBones());
}

void vkGraphicsGL4::SetSkeletonMatrices(const vkMatrix4f *matrices, vkSize numberOfMatrices)
{
  m_skeletonMatrices = matrices;
  m_numberOfSkeletonMatrices = numberOfMatrices;
}

void vkGraphicsGL4::SetSkeletonBoneMapping(const vkUInt32 *boneMapping, vkSize numberOfBoneMappings)
{
  m_skeletonBoneMapping = boneMapping;
  m_numberOfSkeletonBoneMappings = numberOfBoneMappings;
}


void vkGraphicsGL4::SetShadowMatrices(const vkMatrix4f *projView, const vkMatrix4f *proj, const vkMatrix4f *view, const vkVector2f *nearFars, vkSize numberOfMatrices)
{
  m_numberOfShadowMatrices = numberOfMatrices;
  memcpy(m_shadowMatricesProjView, projView, sizeof(vkMatrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesProj, proj, sizeof(vkMatrix4f) * numberOfMatrices);
  memcpy(m_shadowMatricesView, view, sizeof(vkMatrix4f) * numberOfMatrices);
  memcpy(m_shadowNearFars, nearFars, sizeof(vkVector2f) * numberOfMatrices);
}

void vkGraphicsGL4::RecalculateMatrix(vkMatrixType type)
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
    vkMatrix4f::Mult(m_matrices[eMT_MatProj], m_matrices[eMT_MatView], m_matrices[eMT_MatProjView]);
    break;

  case eMT_MatViewModel:
    vkMatrix4f::Mult(m_matrices[eMT_MatView], m_matrices[eMT_MatModel], m_matrices[eMT_MatViewModel]);
    break;

  case eMT_MatProjViewModel:
    RecalculateMatrix(eMT_MatProjView);
    vkMatrix4f::Mult(m_matrices[eMT_MatProjView], m_matrices[eMT_MatModel], m_matrices[eMT_MatProjViewModel]);
    break;

  case eMT_MatProj:
  case eMT_MatView:
  case eMT_MatModel:
  default:
    break;
  }
}

void vkGraphicsGL4::BindSkeleton()
{
  if (m_skeletonMatrices && m_numberOfSkeletonMatrices)
  {

    IShaderAttribute *attrib = m_program->GetAttribute(eVAT_MatsSkeleton);
    if (attrib)
    {
      attrib->Set(m_skeletonMatrices, m_numberOfSkeletonMatrices);
    }
  }
  if (m_skeletonBoneMapping && m_numberOfSkeletonBoneMappings)
  {
    IShaderAttribute *attrib = m_program->GetAttribute(eVAT_SkeletonMapping);
    if (attrib)
    {
      attrib->Set(m_skeletonBoneMapping, m_numberOfSkeletonBoneMappings);
    }
  }
}

void vkGraphicsGL4::BindMatrices()
{
  if (!m_program)
  {
    return;
  }
  for (vkUInt32 i = 0; i < eMT_COUNT; ++i)
  {
    IShaderAttribute *attrib = m_program->GetAttribute(eVAT_MatProj + i);
    if (attrib)
    {
      RecalculateMatrix((vkMatrixType)i);
      attrib->Set(m_matrices[i]);
    }
  }

  
  static vkShaderAttributeID ShadowMatricesProjViewAttribID("ShadowMapMatProjView");
  IShaderAttribute *attrib = m_program->GetAttribute(ShadowMatricesProjViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProjView, m_numberOfShadowMatrices);
  }

  static vkShaderAttributeID ShadowMatricesProjAttribID("ShadowMapMatProj");
  attrib = m_program->GetAttribute(ShadowMatricesProjAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesProj, m_numberOfShadowMatrices);
  }

  static vkShaderAttributeID ShadowMatricesViewAttribID("ShadowMapMatView");
  attrib = m_program->GetAttribute(ShadowMatricesViewAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowMatricesView, m_numberOfShadowMatrices);
  }

  static vkShaderAttributeID ShadowNearFarsAttribID("ShadowMapNearFar");
  attrib = m_program->GetAttribute(ShadowNearFarsAttribID);
  if (attrib)
  {
    attrib->Set(m_shadowNearFars, m_numberOfShadowMatrices);
  }


}


void vkGraphicsGL4::SetVertexDeclaration(IVertexDeclaration *vertexDeclaration)
{
  vkVertexDeclarationGL4 *decl = static_cast<vkVertexDeclarationGL4*>(vertexDeclaration);
  VK_SET(m_vertexDeclaration, decl);
}

void vkGraphicsGL4::SetVertexBuffer(vkUInt16 streamIdx, IVertexBuffer *vertexBuffer)
{
  assert(streamIdx < 16);

  VertexBufferGL4 *vb = static_cast<VertexBufferGL4*>(vertexBuffer);
  VK_SET(m_vertexBuffer[streamIdx], vb);
}

void vkGraphicsGL4::SetIndexBuffer(IIndexBuffer *indexBuffer)
{
  IndexBufferGL4 *ib = static_cast<IndexBufferGL4*>(indexBuffer);
  VK_SET(m_indexBuffer, ib);
}


void vkGraphicsGL4::SetShader(IShader *shader)
{
  VK_CHECK_GL_ERROR;
  vkProgramGL4 *prog = static_cast<vkProgramGL4*>(shader);
  if (prog != m_program)
  {
    VK_SET(m_program, prog);

    if (prog)
    {
      // we use the new program
      prog->Bind();
    }
    else
    {
      glUseProgram(0);
    }
  }
  InvalidateTextures();
  VK_CHECK_GL_ERROR;
}

vkTextureUnit vkGraphicsGL4::BindTexture(ITexture *texture)
{
  VK_CHECK_GL_ERROR;
  if (texture == 0 || m_nextTextureUnit == eTU_Invalid)
  {
    if (m_nextTextureUnit != eTU_Invalid)
    {
      SetTexture(m_nextTextureUnit, 0);
    }
    return eTU_Invalid;
  }

  vkTextureUnit unit = m_nextTextureUnit;
  m_nextTextureUnit = static_cast<vkTextureUnit>(m_nextTextureUnit + 1);
  glActiveTexture(GL_TEXTURE0 + m_nextTextureUnit);
  VK_CHECK_GL_ERROR;
  SetSampler(unit, texture->GetSampler());
  SetTexture(unit, texture);


  VK_CHECK_GL_ERROR;
  return unit;
}

void vkGraphicsGL4::SetTexture(vkTextureUnit unit, ITexture *texture)
{
  VK_CHECK_GL_ERROR;
  vkTextureGL4 *textureGL = texture ? vkQueryClass<vkTextureGL4>(texture) : 0;
  if (m_textures[unit] != textureGL)
  {
    VK_SET(m_textures[unit], textureGL);
    m_textureChanged[unit] = true;
    if (texture)
    {
      glActiveTexture(GL_TEXTURE0 + unit);
      textureGL->Bind();
    }
  }
  VK_CHECK_GL_ERROR;
}

void vkGraphicsGL4::SetSampler(vkTextureUnit unit, ISampler *sampler)
{
  vkSamplerGL4 *samplerGL = sampler ? vkQueryClass<vkSamplerGL4>(sampler) : 0;
  if (m_samplers[unit] != samplerGL)
  {
    VK_SET(m_samplers[unit], samplerGL);
    m_samplerChanged[unit] = true;
    samplerGL->Bind(unit);
  }
}

void vkGraphicsGL4::SetRenderTarget(IRenderTarget *renderTarget)
{
  VK_CHECK_GL_ERROR;
  vkRenderTargetGL4 *rtGL4 = vkQueryClass<vkRenderTargetGL4>(renderTarget);
  VK_CHECK_GL_ERROR;
  if (m_renderTarget != rtGL4 )
  {
    VK_SET(m_renderTarget, rtGL4);
    if (m_renderTarget)
    {
      VK_CHECK_GL_ERROR;
      m_renderTarget->Bind();
      VK_CHECK_GL_ERROR;
    }
    else
    {
      VK_CHECK_GL_ERROR;
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      VK_CHECK_GL_ERROR;
    }
  }
  VK_CHECK_GL_ERROR;
}


void vkGraphicsGL4::SetBlendEnabled(bool enable)
{
  if (m_blendEnabled != enable)
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
  }
}

bool vkGraphicsGL4::IsBlendEnabled() const
{
  return m_blendEnabled;
}

void vkGraphicsGL4::SetBlendMode(vkBlendMode blendSrc, vkBlendMode blendDst)
{
  if (blendSrc != m_blendModeSrcColor || blendSrc != m_blendModeSrcAlpha || blendDst != m_blendModeDstColor || blendDst != m_blendModeDstAlpha)
  {
    m_blendModeSrcColor = m_blendModeSrcAlpha = blendSrc;
    m_blendModeDstColor = m_blendModeDstAlpha = blendDst;
    glBlendFunc(blendModeMap[blendSrc], blendModeMap[blendDst]);
  }
}

void vkGraphicsGL4::SetBlendMode(vkBlendMode blendSrcColor, vkBlendMode blendSrcAlpha, vkBlendMode blendDstColor, vkBlendMode blendDstAlpha)
{
  if (blendSrcColor != m_blendModeSrcColor || blendSrcAlpha != m_blendModeSrcAlpha || blendDstColor != m_blendModeDstColor || blendDstAlpha != m_blendModeDstAlpha)
  {
    m_blendModeSrcColor = blendSrcColor;
    m_blendModeSrcAlpha = blendSrcAlpha;
    m_blendModeDstColor = blendDstColor;
    m_blendModeDstAlpha = blendDstAlpha;
    glBlendFuncSeparate(blendModeMap[blendSrcColor], blendModeMap[blendSrcAlpha], blendModeMap[blendDstColor], blendModeMap[blendDstAlpha]);
  }
}

void vkGraphicsGL4::GetBlendMode(vkBlendMode &blendSrcColor, vkBlendMode &blendDstColor, vkBlendMode &blendSrcAlpha, vkBlendMode &blendDstAlpha) const
{
  blendSrcColor = m_blendModeSrcColor;
  blendSrcAlpha = m_blendModeSrcAlpha;
  blendDstColor = m_blendModeDstColor;
  blendDstAlpha = m_blendModeDstAlpha;
}


void vkGraphicsGL4::SetDepthMask(bool depth)
{
  //if (m_depthMask != depth)
  {
    m_depthMask = depth;
    glDepthMask(m_depthMask);
  }
}

void vkGraphicsGL4::SetColorMask(bool red, bool green, bool blue, bool alpha)
{
  vkUInt8 colorMask = (red ? 0x8 : 0x0) | (green ? 0x4 : 0x0) | (blue ? 0x2 : 0x0) | (alpha ? 0x1 : 0x0);
//  if (colorMask != m_colorMask)
  {
    m_colorMask = colorMask;
    glColorMask(red, green, blue, alpha);
  }
}

void vkGraphicsGL4::SetDepthTest(bool depthTest)
{
  //if (m_depthTest != depthTest)
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
  }
}

void vkGraphicsGL4::SetDepthFunc(vkCompareMode compareMode)
{
  //if (m_depthFunc != compareMode)
  {
    m_depthFunc = compareMode;
    glDepthFunc(compareFuncMap[compareMode]);
  }
}

void vkGraphicsGL4::SetRenderFadeInOut(float near, float far)
{
  m_fadeInOutDistances.Set(near, far, far - near);
}

void vkGraphicsGL4::SetRenderFadeInOutValue(vkUInt8 value)
{
  m_fadeInOutValue = value;
}

void vkGraphicsGL4::SetRenderDestination(vkRenderDestination renderDestination)
{
  glDrawBuffer(renderDestinationMap[renderDestination]);
}

void vkGraphicsGL4::SetRenderDestinations(vkRenderDestination *renderDestination, vkSize numRenderDestinations)
{
  static GLenum destinations[16];
  if (numRenderDestinations > 16)
  {
    numRenderDestinations = 16;
  }
  \
  for (vkSize i = 0; i < numRenderDestinations; ++i)
  {
    destinations[i] = renderDestinationMap[renderDestination[i]];
  }
  glDrawBuffers(numRenderDestinations, destinations);
}

void vkGraphicsGL4::SetClearColorValue(const vkVector4f &colorValue)
{
  if (colorValue != m_clearColor)
  {
    m_clearColor = colorValue;
    glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
  }
}

void vkGraphicsGL4::SetClearDepthValue(float depthValue)
{
  if (depthValue != m_clearDepth)
  {
    m_clearDepth = depthValue;
    glClearDepth(m_clearDepth);
  }
}

void vkGraphicsGL4::SetClearStencilValue(vkUInt8 stencilValue)
{
  if (stencilValue != m_clearStencil)
  {
    m_clearStencil = stencilValue;
    glClearStencil(m_clearStencil);
  }
}


void vkGraphicsGL4::Clear(bool clearColor, const vkVector4f &color, bool clearDepth, float depth, bool clearStencil, vkUInt8 stencil)
{
  VK_CHECK_GL_ERROR;
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
  VK_CHECK_GL_ERROR;

}

void vkGraphicsGL4::SetViewport(vkUInt16 width, vkUInt16 height)
{
  SetViewport(0, 0, width, height);
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void vkGraphicsGL4::SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height)
{
  glViewport(x, y, width, height);
  m_viewportWidth = width;
  m_viewportHeight = height;
}

void vkGraphicsGL4::SetViewport(IRenderTarget *renderTarget)
{
  SetViewport(0, 0, renderTarget->GetWidth(), renderTarget->GetHeight());
}


void vkGraphicsGL4::BindValues()
{
  VK_CHECK_GL_ERROR;
  BindMatrices();



  /* ******************************************* */
  /*    Bind data for the fading gradient        */
  /* ******************************************* */
  IShaderAttribute *attribBinaryGradient = m_program->GetAttribute(eVAT_BinaryGradient);
  if (attribBinaryGradient)
  {
    vkTextureUnit tu = BindTexture(vkBinaryGradient::GetBinaryGradient());
    if (tu != eTU_Invalid)
    {
      attribBinaryGradient->Set(tu);
    }
  }

  IShaderAttribute *attributeBinaryGradientToScreen = m_program->GetAttribute(eVAT_BinaryGradientToScreen);
  if (attributeBinaryGradientToScreen)
  {
    attributeBinaryGradientToScreen->Set((float)m_viewportWidth / 8.0f, (float)m_viewportHeight / 8.0f);
  }

  IShaderAttribute *attributeFadeInOutValue = m_program->GetAttribute(eVAT_FadeInOutValue);
  if (attributeFadeInOutValue)
  {
    attributeFadeInOutValue->Set(m_fadeInOutValue);
  }
  IShaderAttribute *attributeFadeInOutDist = m_program->GetAttribute(eVAT_FadeInOutDist);
  if (attributeFadeInOutDist)
  {
    attributeFadeInOutDist->Set(m_fadeInOutDistances);
  }

  /* ******************************************* */
  /*    Bind other data                          */
  /* ******************************************* */
  IShaderAttribute *attributeViewportSize = m_program->GetAttribute(eVAT_ViewportSize);
  if (attributeViewportSize)
  {
    attributeViewportSize->Set((float)m_viewportWidth, (float)m_viewportHeight);
  }

  IShaderAttribute *attributeViewportSizeInv = m_program->GetAttribute(eVAT_ViewportSizeInv);
  if (attributeViewportSizeInv)
  {
    attributeViewportSizeInv->Set(1.0f / (float)m_viewportWidth, 1.0f / (float)m_viewportHeight);
  }
  VK_CHECK_GL_ERROR;

}


void vkGraphicsGL4::Render(vkPrimitiveType primType, vkUInt32 count)
{
  BindValues();
  VK_CHECK_GL_ERROR;
  if (BindVertexDeclaration())
  {
    glDrawArrays(primitiveTypeMap[primType], 0, count);

    UnbindVertexDeclaration();
  }
  VK_CHECK_GL_ERROR;
}

void vkGraphicsGL4::RenderIndexed(vkPrimitiveType primType, vkUInt32 count, vkDataType indexType)
{
  BindValues();
  VK_CHECK_GL_ERROR;
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    glDrawElements(primitiveTypeMap[primType], count, dataTypeMap[indexType], 0);

    UnbindVertexDeclaration();
  }
  VK_CHECK_GL_ERROR;
}

void vkGraphicsGL4::RenderFullScreenFrame(ITexture2D *texture)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture);
}

void vkGraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, ITexture2D *texture)
{
  VK_CHECK_GL_ERROR;
  static vkShaderAttributeID attrDiffuseID("Diffuse");
  static vkShaderAttributeID attrLeftBottomID("LeftBottom");
  static vkShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  glDepthMask(true);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_ALWAYS);
  glClearDepth(1.0);
  glDisable(GL_BLEND);

  SetShader(m_fullScreenProgram);
  SetVertexBuffer(0, m_fullScreenParamVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  vkTextureUnit tu = BindTexture(texture);
  if (m_fullScreenProgram->GetAttribute(attrDiffuseID))
  {
    m_fullScreenProgram->GetAttribute(attrDiffuseID)->Set(tu);
  }
  m_fullScreenProgram->GetAttribute(attrLeftBottomID)->Set(vkVector2f(x0, y0));
  m_fullScreenProgram->GetAttribute(attrDeltaID)->Set(vkVector2f(x1 - x0, y1 - y0));

  Render(ePT_Triangles, 6);
  VK_CHECK_GL_ERROR;
}

void vkGraphicsGL4::RenderFullScreenFrame(ITexture2DArray *texture, int layer)
{
  RenderFullScreenFrame(0.0f, 1.0f, 0.0f, 1.0f, texture, layer);
}

void vkGraphicsGL4::RenderFullScreenFrame(float left, float right, float bottom, float top, ITexture2DArray *texture, int layer)
{
  VK_CHECK_GL_ERROR;
  static vkShaderAttributeID attrDiffuseID("Diffuse");
  static vkShaderAttributeID attrLayerID("Layer");
  static vkShaderAttributeID attrLeftBottomID("LeftBottom");
  static vkShaderAttributeID attrDeltaID("Delta");

  float x0 = -1.0f + left * 2.0f;
  float y0 = -1.0f + bottom * 2.0f;
  float x1 = -1.0f + right * 2.0f;
  float y1 = -1.0f + top * 2.0f;

  SetDepthMask(true);
  SetDepthTest(true);
  SetDepthFunc(eCM_Always);
  SetDepthMask(false);
  glClearDepth(1.0);

  SetShader(m_fullScreenArrayProgram);
  SetVertexBuffer(0, m_fullScreenParamVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  vkTextureUnit tu = BindTexture(texture);
  m_fullScreenArrayProgram->GetAttribute(attrDiffuseID)->Set(tu);
  m_fullScreenArrayProgram->GetAttribute(attrLeftBottomID)->Set(vkVector2f(x0, y0));
  m_fullScreenArrayProgram->GetAttribute(attrDeltaID)->Set(vkVector2f(x1 - x0, y1 - y0));
  m_fullScreenArrayProgram->GetAttribute(attrLayerID)->Set(layer);
  Render(ePT_Triangles, 6);
  VK_CHECK_GL_ERROR;
}


void vkGraphicsGL4::RenderFullScreenFrame()
{
  VK_CHECK_GL_ERROR;
  SetDepthMask(true);
  SetDepthTest(false);
  SetDepthFunc(eCM_Always);
  SetDepthMask(false);
  //glClearDepth(1.0);

  SetVertexBuffer(0, m_fullScreenVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  Render(ePT_Triangles, 6);
  VK_CHECK_GL_ERROR;
}


bool vkGraphicsGL4::BindVertexDeclaration()
{
  VK_CHECK_GL_ERROR;
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

  VK_CHECK_GL_ERROR;
  return true;
}

void vkGraphicsGL4::UnbindVertexDeclaration()
{
  VK_CHECK_GL_ERROR;
  VertexBufferGL4::Unbind();
  for (unsigned i = 0, in = m_vertexDeclaration->GetNumberOfStreams(); i < in; ++i)
  {
    m_vertexDeclaration->UnbindStream(m_program, i);
  }

  VK_CHECK_GL_ERROR;
}


void vkGraphicsGL4::FreeTextures()
{
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    SetTexture((vkTextureUnit)i, 0);
  }
}

void vkGraphicsGL4::InvalidateSamplers()
{
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_samplerChanged[i] = true;
  }
}


void vkGraphicsGL4::InvalidateTextures()
{
  m_nextTextureUnit = eTU_TextureUnit0;
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_textureChanged[i] = true;
  }
}


void vkGraphicsGL4::InitFullScreenData()
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

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, 6 * sizeof(float), 0),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 4 * sizeof(float), 6 * sizeof(float), 0),
    vkVertexElement(),
  };

  m_fullScreenVertexDeclaration = static_cast<vkVertexDeclarationGL4*>(CreateVertexDeclaration(elements));

  m_fullScreenProgram = vkResourceManager::Get()->GetOrLoad<vkProgramGL4>(vkResourceLocator("${shaders}/renderer/SimplePresent.xasset"));
  m_fullScreenArrayProgram = vkResourceManager::Get()->GetOrLoad<vkProgramGL4>(vkResourceLocator("${shaders}/renderer/ArrayPresent.xasset "));
}


void vkGraphicsGL4::AssetGraphicsErrors() const
{
  VK_CHECK_GL_ERROR;
}