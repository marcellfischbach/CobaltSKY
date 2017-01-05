#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/vktypes.hh>
#include <Valkyrie/vkenums.hh>

#include <Valkyrie/graphics/ishaderstream.refl.hh>

VK_INTERFACE();
struct VKE_API IShaderStream : public IObject
{
  VK_CLASS_GEN;

  virtual ~IShaderStream() { }

  virtual const vkString &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(vkSize size, vkSize stride, vkSize offset, vkDataType type) = 0;
  virtual void Set(vkSize size, vkSize stride, const void *data, vkDataType type) = 0;

  virtual void SetFrequency(vkUInt8 frequency) = 0;

  virtual void Enable() = 0;
  virtual void Disable() = 0;

};