#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/vkenums.hh>

#include <valkyrie/graphics/ishaderstream.refl.hh>

VK_INTERFACE();
struct VKE_API iShaderStream : public iObject
{
  VK_CLASS_GEN;

  virtual ~iShaderStream() { }

  virtual const vkString &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(vkSize size, vkSize stride, vkSize offset, vkDataType type) = 0;
  virtual void Set(vkSize size, vkSize stride, const void *data, vkDataType type) = 0;

  virtual void SetFrequency(vkUInt8 frequency) = 0;

  virtual void Enable() = 0;
  virtual void Disable() = 0;

};