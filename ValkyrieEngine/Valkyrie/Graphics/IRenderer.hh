#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Graphics/IRenderer.refl.hh>
#include <Valkyrie/Enums.hh>

struct IIndexBuffer;
struct IVertexBuffer;
struct IShader;

VK_INTERFACE()
struct VKE_API IRenderer : public IObject
{
  VK_CLASS_GEN;


  /** 
   * \name Create of render buffers and render states
   * @{
   */
  virtual IIndexBuffer *CreateIndexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  virtual IVertexBuffer *CreateVertexBuffer(vkSize size, const void *data, vkBufferDataMode mode) = 0;
  /**
   * @}
   */


};

