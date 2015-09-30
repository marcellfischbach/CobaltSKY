
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

