#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Graphics/iindexbuffer.refl.hh>

VK_INTERFACE();
struct VKE_API IIndexBuffer : public IObject
{
  VK_CLASS_GEN;

  virtual vkSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, vkSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};