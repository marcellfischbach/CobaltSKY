#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/csenums.hh>

#include <cobalt/graphics/ishaderstream.refl.hh>

CS_INTERFACE();
struct CSE_API iShaderStream : public iObject
{
  CS_CLASS_GEN;

  virtual ~iShaderStream() { }

  virtual const csString &GetName() const = 0;

  virtual void SetArrayIndex(csUInt32 idx) = 0;

  virtual void Set(csSize size, csSize stride, csSize offset, csDataType type) = 0;
  virtual void Set(csSize size, csSize stride, const void *data, csDataType type) = 0;

  virtual void SetFrequency(csUInt8 frequency) = 0;

  virtual void Enable() = 0;
  virtual void Disable() = 0;

};