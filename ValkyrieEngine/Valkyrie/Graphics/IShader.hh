#pragma once


#include <Valkyrie/Export.hh>
#include <Valkyrie/Core/Matrix.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Core/String.hh>
#include <Valkyrie/Core/Vector.hh>
#include <Valkyrie/Graphics/IShader.refl.hh>
#include <Valkyrie/Graphics/ShaderID.hh>
#include <Valkyrie/Enums.hh>
using std::string;

VK_INTERFACE();
struct VKE_API IShaderAttribute : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ~IShaderAttribute() {}

  virtual const vkString &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(float x) = 0;
  virtual void Set(float x, float y) = 0;
  virtual void Set(float x, float y, float z) = 0;
  virtual void Set(float x, float y, float z, float w) = 0;

  virtual void Set(vkInt32 x) = 0;
  virtual void Set(vkInt32 x, vkInt32 y) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z, vkInt32 w) = 0;

  virtual void Set(const vkVector2f &v) = 0;
  virtual void Set(const vkVector3f &v) = 0;
  virtual void Set(const vkVector4f &v) = 0;

  virtual void Set(const vkMatrix4f &m) = 0;


};

VK_INTERFACE();
struct VKE_API IShaderStream : public virtual IObject
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

VK_INTERFACE();
struct VKE_API IShader : public virtual IObject
{
  VK_CLASS_GEN;

  virtual ~IShader() { }

  virtual void RegisterAttribute(const vkShaderAttributeID &id) = 0;
  virtual void RegisterStream(const vkShaderStreamID &id) = 0;

  virtual vkUInt32 GetNumberOfAttributes() const = 0;
  virtual IShaderAttribute *GetAttribute(const vkShaderAttributeID &id) = 0;

  virtual vkUInt16 GetNumberOfStreams() const = 0;
  virtual IShaderStream *GetStream(const vkShaderStreamID &id) = 0;

};

