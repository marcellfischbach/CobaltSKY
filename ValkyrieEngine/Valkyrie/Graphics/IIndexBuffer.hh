#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IIndexBuffer.refl.hh>
#include <Valkyrie/Enums.hh>

VK_INTERFACE();
struct VKE_API IIndexBuffer : public IObject
{
  VK_CLASS_GEN;

  virtual vkSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, vkSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};