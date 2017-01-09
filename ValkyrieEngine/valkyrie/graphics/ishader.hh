#pragma once


#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/core/vkstring.hh>
#include <valkyrie/graphics/vkshaderattributeid.hh>
#include <valkyrie/graphics/vkshaderstreamid.hh>
#include <valkyrie/vkenums.hh>
#include <valkyrie/graphics/ishader.refl.hh>

struct iShaderAttribute;
struct iShaderStream;


VK_INTERFACE();
struct VKE_API iShader : public iObject
{
  VK_CLASS_GEN;

  virtual ~iShader() { }

  virtual void RegisterAttribute(const vkShaderAttributeID &id) = 0;
  virtual void RegisterStream(const vkShaderStreamID &id) = 0;

  virtual vkUInt32 GetNumberOfAttributes() const = 0;
  virtual iShaderAttribute *GetAttribute(const vkShaderAttributeID &id) = 0;
  virtual iShaderAttribute *GetAttribute(const vkString &attributeName) = 0;

  virtual vkUInt16 GetNumberOfStreams() const = 0;
  virtual iShaderStream *GetStream(const vkShaderStreamID &id) = 0;
  virtual iShaderStream *GetStream(const vkString &streamName) = 0;

};

