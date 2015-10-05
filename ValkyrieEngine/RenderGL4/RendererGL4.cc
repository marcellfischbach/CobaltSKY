
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/IndexBufferGL4.hh>
#include <RenderGL4/VertexBufferGL4.hh>
#include <RenderGL4/VertexDeclarationGL4.hh>
#include <RenderGL4/Shader.hh>
#include <RenderGL4/ShaderLoader.hh>
#include <RenderGL4/MappingGL4.hh>
#include <RenderGL4/DefinesGL4.hh>
#include <GL/glew.h>
#include <assert.h>


RendererGL4::RendererGL4()
  : IRenderer()
  , m_vertexDeclaration(0)
  , m_indexBuffer(0)
  , m_program(0)
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


  vkResourceManager::Get()->RegisterLoader(new vkShaderGL4Loader());
  vkResourceManager::Get()->RegisterLoader(new vkProgramGL4Loader());
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

void RendererGL4::SetProjectionMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatProj] = matrix;
  m_matrixNeedsRecalculation[eMT_MatProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewProj] = true;
  m_matrixNeedsRecalculation[eMT_MatViewProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProj] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProjInv] = true;
}

void RendererGL4::SetViewMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatView] = matrix;
  m_matrixNeedsRecalculation[eMT_MatViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatViewProj] = true;
  m_matrixNeedsRecalculation[eMT_MatViewProjInv] = true;
  m_matrixNeedsRecalculation[eMT_MatModelView] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProj] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProjInv] = true;

}

void RendererGL4::SetModelMatrix(const vkMatrix4f &matrix)
{
  m_matrices[eMT_MatModel] = matrix;
  m_matrixNeedsRecalculation[eMT_MatModelInv] = true;
  m_matrixNeedsRecalculation[eMT_MatModelView] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewInv] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProj] = true;
  m_matrixNeedsRecalculation[eMT_MatModelViewProjInv] = true;
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
    vkMatrix4f::Invert(m_matrices[eMT_MatProj], m_matrices[eMT_MatProjInv]);
    break;
  case eMT_MatViewInv:
    vkMatrix4f::Invert(m_matrices[eMT_MatView], m_matrices[eMT_MatViewInv]);
    break;
  case eMT_MatModelInv:
    vkMatrix4f::Invert(m_matrices[eMT_MatModel], m_matrices[eMT_MatModelInv]);
    break;

  case eMT_MatViewProjInv:
    RecalculateMatrix(eMT_MatViewProj);
    vkMatrix4f::Invert(m_matrices[eMT_MatViewProj], m_matrices[eMT_MatViewProjInv]);
    break;
  case eMT_MatModelViewInv:
    RecalculateMatrix(eMT_MatModelView);
    vkMatrix4f::Invert(m_matrices[eMT_MatModelView], m_matrices[eMT_MatModelViewInv]);
    break;
  case eMT_MatModelViewProjInv:
    RecalculateMatrix(eMT_MatModelViewProj);
    vkMatrix4f::Invert(m_matrices[eMT_MatModelViewProj], m_matrices[eMT_MatModelViewProjInv]);
    break;

  case eMT_MatViewProj:
    vkMatrix4f::Mult(m_matrices[eMT_MatProj], m_matrices[eMT_MatView], m_matrices[eMT_MatViewProj);
    break;

  case eMT_MatProj:
  case eMT_MatView:
  case eMT_MatModel:
  default:
    break;
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
  VK_SET(m_program, prog);

  if (prog)
  {
    // we use the new program
    prog->Bind();
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
  if (BindVertexDeclaration())
  {

    UnbindVertexDeclaration();
  }
}

void RendererGL4::RenderIndexed(vkPrimitiveType primType, vkUInt32 count, vkDataType indexType)
{
  VK_CHECK_GL_ERROR;
  if (BindVertexDeclaration())
  {
    m_indexBuffer->Bind();
    glDrawElements(primitiveTypeMap[primType], count, dataTypeMap[indexType], 0);

    UnbindVertexDeclaration();
  }
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

