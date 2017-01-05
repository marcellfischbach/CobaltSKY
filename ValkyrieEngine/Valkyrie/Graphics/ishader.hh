#pragma once


#include <Valkyrie/vkexport.hh>
#include <Valkyrie/core/vkclass.hh>
#include <Valkyrie/core/vkstring.hh>
#include <Valkyrie/Graphics/ShaderID.hh>
#include <Valkyrie/vkenums.hh>
#include <Valkyrie/Graphics/IShader.refl.hh>

struct IShaderAttribute;
struct IShaderStream;


VK_INTERFACE();
struct VKE_API IShader : public IObject
{
  VK_CLASS_GEN;

  virtual ~IShader() { }

  virtual void RegisterAttribute(const vkShaderAttributeID &id) = 0;
  virtual void RegisterStream(const vkShaderStreamID &id) = 0;

  virtual vkUInt32 GetNumberOfAttributes() const = 0;
  virtual IShaderAttribute *GetAttribute(const vkShaderAttributeID &id) = 0;
  virtual IShaderAttribute *GetAttribute(const vkString &attributeName) = 0;

  virtual vkUInt16 GetNumberOfStreams() const = 0;
  virtual IShaderStream *GetStream(const vkShaderStreamID &id) = 0;
  virtual IShaderStream *GetStream(const vkString &streamName) = 0;

};

