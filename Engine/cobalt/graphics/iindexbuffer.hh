#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/csenums.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/graphics/iindexbuffer.refl.hh>

namespace cs
{

CS_CLASS();
struct CSE_API iIndexBuffer : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iIndexBuffer() { }

  virtual csSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, csSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, cs::eBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};

}
