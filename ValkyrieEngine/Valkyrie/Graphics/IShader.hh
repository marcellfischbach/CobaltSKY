#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IShader.refl.hh>
#include <Valkyrie/Enums.hh>
#include <string>

using std::string;

VK_INTERFACE();
struct IShaderAttribute : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ~IShaderAttribute() {}

  virtual const string &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(float x) = 0;
  virtual void Set(float x, float y) = 0;
  virtual void Set(float x, float y, float z) = 0;
  virtual void Set(float x, float y, float z, float w) = 0;

  virtual void Set(vkInt32 x) = 0;
  virtual void Set(vkInt32 x, vkInt32 y) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z, vkInt32 w) = 0;



};

VK_INTERFACE();
struct IShaderStream : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ~IShaderStream() { }

  virtual const string &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(vkSize size, vkSize stride, vkSize offset, vkDataType type) = 0;
  virtual void Set(vkSize size, vkSize stride, const void *data, vkDataType type) = 0;

  virtual void SetFrequency(vkUInt8 frequency) = 0;

  virtual void Enable() = 0;
  virtual void Disable() = 0;

};

VK_INTERFACE();
struct VKE_API IShader : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ~IShader() { }

  virtual vkUInt32 GetNumberOfAttributes() const = 0;
  virtual IShaderAttribute *GetAttribute(vkUInt32 idx) = 0;
  virtual IShaderAttribute *GetAttribute(const string &name) = 0;
  virtual IShaderAttribute *GetAttribute(vkVertexAttributeType attr) = 0;

  virtual vkUInt16 GetNumberOfStreams() const = 0;
  virtual IShaderStream *getStream(vkUInt16 idx) = 0;
  virtual IShaderStream *GetStream(const string &name) = 0;
  virtual IShaderStream *GetStream(vkVertexStreamType stream) = 0;

};

