#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/ivertexbuffer.refl.hh>

VK_INTERFACE();
struct VKE_API iVertexBuffer : public iObject
{
  VK_CLASS_GEN;

  virtual vkSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, vkSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, vkBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};