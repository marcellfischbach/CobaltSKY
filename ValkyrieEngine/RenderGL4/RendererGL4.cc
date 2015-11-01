
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/RenderTargetGL4.hh>
#include <RenderGL4/IndexBufferGL4.hh>
#include <RenderGL4/VertexBufferGL4.hh>
#include <RenderGL4/VertexDeclarationGL4.hh>
#include <RenderGL4/Shader.hh>
#include <RenderGL4/ShaderLoader.hh>
#include <RenderGL4/TextureGL4.hh>
#include <RenderGL4/MappingGL4.hh>
#include <RenderGL4/DefinesGL4.hh>
#include <RenderGL4/Deferred/DeferredFrameProcessor.hh>
#include <GL/glew.h>
#include <assert.h>


RendererGL4::RendererGL4()
  : IRenderer()
  , m_vertexDeclaration(0)
  , m_indexBuffer(0)
  , m_program(0)
  , m_renderTarget(0)
{
  VK_CLASS_GEN_CONSTR;
  VK_CHECK_GL_ERROR;
  glewExperimental = true;
  if (glewInit() != GLEW_OK)
  {
    printf("Initialize GLEW failed\n");
  }
  glGetError();
  VK_CHECK_GL_ERROR;
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

  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  memset(m_samplers, 0, sizeof(m_samplers));
  memset(m_textures, 0, sizeof(m_textures));
  InvalidateSamplers();
  InvalidateTextures();

  vkResourceManager::Get()->RegisterLoader(new vkShaderGL4Loader());
  vkResourceManager::Get()->RegisterLoader(new vkProgramGL4Loader());

  InitFullScreenData();
}



IIndexBuffer *RendererGL4::CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode)
{
  IndexBufferGL4 *indexBuffer = new IndexBufferGL4();
  if (!indexBuffer->CreateBuffer(size, data, mode))
  {
    indexBuffer->Release();
    indexBuffer = 0;
  }

  return indexBuffer;
}



IVertexBuffer *RendererGL4::CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode)
{
  VertexBufferGL4 *vertexBuffer = new VertexBufferGL4();
  if (!vertexBuffer->CreateBuffer(size, data, mode))
  {
    vertexBuffer->Release();
    vertexBuffer = 0;
  }

  return vertexBuffer;
}

IVertexDeclaration *RendererGL4::CreateVertexDeclaration(const vkVertexElement *elements)
{
  vkVertexDeclarationGL4 *decl = new vkVertexDeclarationGL4();
  if (!decl->Create(elements))
  {
    decl->Release();
    decl = 0;
  }

  return decl;
}

IRenderTarget *RendererGL4::CreateRenderTarget()
{
  return new vkRenderTargetGL4();
}

ISampler *RendererGL4::CreateSampler()
{
  vkSamplerGL4 *sampler = new vkSamplerGL4();
  if (!sampler->Initialize())
  {
    sampler->Release();
    sampler = 0;
  }

  return sampler;
}

ITexture2D *RendererGL4::CreateTexture2D(vkPixelFormat format, vkUInt16 width, vkUInt16 height)
{
  vkTexture2DGL4 *texture = new vkTexture2DGL4();
  if (!texture->Initialize(format, width, height))
  {
    texture->Release();
    texture = 0;
  }
  return texture;
}

IFrameProcessor *RendererGL4::CreateDeferredFrameProcessor()
{
  return vkDeferredFrameProcessor();
}



void RendererGL4::SetProjectionMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatProj] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjView] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void RendererGL4::SetViewMatrix(const vkMatrix4f &matrix)
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

void RendererGL4::SetModelMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatModel] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatViewModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModel] = true;
  m_matrixNeedsRecalculation[eMT_MatProjViewModelInv] = true;
}

void RendererGL4::RecalculateMatrix(vkMatrixType type)
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

void RendererGL4::BindMatrices()
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
}

void RendererGL4::SetVertexDeclaration(IVertexDeclaration *vertexDeclaration)
{
  vkVertexDeclarationGL4 *decl = static_cast<vkVertexDeclarationGL4*>(vertexDeclaration);
  VK_SET(m_vertexDeclaration, decl);
}


void RendererGL4::SetVertexBuffer(vkUInt16 streamIdx, IVertexBuffer *vertexBuffer)
{
  assert(streamIdx < 16);

  VertexBufferGL4 *vb = static_cast<VertexBufferGL4*>(vertexBuffer);
  VK_SET(m_vertexBuffer[streamIdx], vb);
}

void RendererGL4::SetIndexBuffer(IIndexBuffer *indexBuffer)
{
  IndexBufferGL4 *ib = static_cast<IndexBufferGL4*>(indexBuffer);
  VK_SET(m_indexBuffer, ib);
}


void RendererGL4::SetShader(IShader *shader)
{
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
}

vkTextureUnit RendererGL4::BindTexture(ITexture *texture)
{
  if (texture == 0 || m_nextTextureUnit == eTU_Invalid)
  {
    return eTU_Invalid;
  }

  vkTextureUnit unit = m_nextTextureUnit;
  m_nextTextureUnit = static_cast<vkTextureUnit>(m_nextTextureUnit + 1);
  glActiveTexture(GL_TEXTURE0 + m_nextTextureUnit);
  VK_CHECK_GL_ERROR;
  SetSampler(unit, texture->GetSampler());
  SetTexture(unit, texture);


  return unit;
}

void RendererGL4::SetTexture(vkTextureUnit unit, ITexture *texture)
{
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
}

void RendererGL4::SetSampler(vkTextureUnit unit, ISampler *sampler)
{
  vkSamplerGL4 *samplerGL = sampler ? vkQueryClass<vkSamplerGL4>(sampler) : 0;
  if (m_samplers[unit] != samplerGL)
  {
    VK_SET(m_samplers[unit], samplerGL);
    m_samplerChanged[unit] = true;
    samplerGL->Bind(unit);
  }
}

void RendererGL4::SetRenderTarget(IRenderTarget *renderTarget)
{
  vkRenderTargetGL4 *rtGL4 = vkQueryClass<vkRenderTargetGL4>(renderTarget);
  if (m_renderTarget != rtGL4)
  {
    VK_SET(m_renderTarget, rtGL4);
    if (m_renderTarget)
    {
      m_renderTarget->Bind();
    }
    else
    {
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
  }

}

void RendererGL4::Clear()
{
  glClearColor(0.0f, 0.0f, 0.5, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererGL4::SetViewport(vkInt16 x, vkInt16 y, vkUInt16 width, vkUInt16 height)
{
  glViewport(x, y, width, height);
}



void RendererGL4::Render(vkPrimitiveType primType, vkUInt32 count)
{
  BindMatrices();
  VK_CHECK_GL_ERROR;
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    glDrawArrays(primitiveTypeMap[primType], 0, count);

    UnbindVertexDeclaration();
  }
}

void RendererGL4::RenderIndexed(vkPrimitiveType primType, vkUInt32 count, vkDataType indexType)
{
  BindMatrices();
  VK_CHECK_GL_ERROR;
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    glDrawElements(primitiveTypeMap[primType], count, dataTypeMap[indexType], 0);

    UnbindVertexDeclaration();
  }
}

void RendererGL4::RenderFullScreenFrame(ITexture2D *texture)
{
  static vkShaderAttributeID diffuse("Diffuse");

  SetShader(m_fullScreenProgram);
  SetVertexBuffer(0, m_fullScreenVertexBuffer);
  SetVertexDeclaration(m_fullScreenVertexDeclaration);
  vkTextureUnit tu = BindTexture(texture);
  m_fullScreenProgram->GetAttribute(diffuse)->Set(tu);
  Render(ePT_Triangles, 6);
}


bool RendererGL4::BindVertexDeclaration()
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

void RendererGL4::UnbindVertexDeclaration()
{
  VertexBufferGL4::Unbind();
  for (unsigned i = 0, in = m_vertexDeclaration->GetNumberOfStreams(); i < in; ++i)
  {
    m_vertexDeclaration->UnbindStream(m_program, i);
  }

}



void RendererGL4::InvalidateSamplers()
{
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_samplerChanged[i] = true;
  }
}


void RendererGL4::InvalidateTextures()
{
  m_nextTextureUnit = eTU_TextureUnit0;
  for (int i = 0; i < eTU_COUNT; ++i)
  {
    m_textureChanged[i] = true;
  }
}


void RendererGL4::InitFullScreenData()
{
  float vertexData[] = {
    -1, -1, 0, 1,     0, 0,
    -1,  1, 0, 1,     0, 1,
     1,  1, 0, 1,     1, 1,

    -1, -1, 0, 1,     0, 0,
     1,  1, 0, 1,     1, 1, 
     1, -1, 0, 1,     1, 0,
  };
  m_fullScreenVertexBuffer = static_cast<VertexBufferGL4*>(CreateVertexBuffer(sizeof(vertexData), vertexData, eBDM_Static));

  vkVertexElement elements[] = {
    vkVertexElement(eVST_Position, eDT_Float, 4, 0, 6 * sizeof(float), 0),
    vkVertexElement(eVST_TexCoord0, eDT_Float, 2, 4 * sizeof(float), 6 * sizeof(float), 0),
    vkVertexElement(),
  };

  m_fullScreenVertexDeclaration = static_cast<vkVertexDeclarationGL4*>(CreateVertexDeclaration(elements));

  m_fullScreenProgram = vkResourceManager::Get()->GetOrLoad<vkProgramGL4>(vkResourceLocator("${shaders}/renderer/renderer.xml:SimplePresent"));
}