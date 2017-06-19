#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/graphics/iindexbuffer.refl.hh>

CS_INTERFACE();
struct CSE_API iIndexBuffer : public iObject
{
  CS_CLASS_GEN;

  virtual csSize GetSize() const = 0;

  virtual bool Copy(unsigned offset, csSize size, const void* data) = 0;

  virtual bool Lock(unsigned offset, void** data, csBufferAccessMode mode) = 0;

  virtual bool Unlock() = 0;

};