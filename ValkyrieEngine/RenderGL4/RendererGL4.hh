
#pragma once

#include <RenderGL4/Export.hh>
#include <Valkyrie/Graphics/IRenderer.hh>
#include <RenderGL4/RendererGL4.refl.hh>


VK_CLASS();
class VKGL4_API RendererGL4 : public IRenderer
{
  VK_CLASS_GEN_OBJECT;
public:
  RendererGL4();

  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode);
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode);

};

