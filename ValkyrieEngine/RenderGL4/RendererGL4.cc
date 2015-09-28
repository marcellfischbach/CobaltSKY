
#include <RenderGL4/RendererGL4.hh>
#include <RenderGL4/IndexBufferGL4.hh>
#include <RenderGL4/VertexBufferGL4.hh>
#include <RenderGL4/ShaderLoader.hh>
#include <GL/glew.h>


RendererGL4::RendererGL4()
{
  VK_CLASS_GEN_CONSTR;
  glewInit();


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

