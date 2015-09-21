#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IVertexBuffer.refl.hh>
#include <Valkyrie/Enums.hh>

VK_INTERFACE();
struct VKE_API IVertexBuffer : public IObject
{
  VK_CLASS_GEN;

  virtual unsigned GetSize() const = 0;

  virtual bool Copy(unsigned offset, unsigned size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

  virtual void Bind() = 0;
  virtual void Unbind() = 0;
};