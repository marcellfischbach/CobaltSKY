#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/csenums.hh>
#include <cobalt/graphics/ivertexbuffer.refl.hh>

CS_INTERFACE();
struct CSE_API iVertexBuffer : public iObject
{
  CS_CLASS_GEN;

  virtual csSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, csSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, csBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};