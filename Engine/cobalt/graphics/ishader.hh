#pragma once


#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/csshaderattributeid.hh>
#include <cobalt/graphics/csshaderstreamid.hh>
#include <cobalt/csenums.hh>
#include <string>

#include <cobalt/graphics/ishader.refl.hh>


namespace cs
{
struct iShaderAttribute;
struct iShaderStream;


CS_CLASS();
struct CSE_API iShader : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iShader() { }

  virtual void RegisterAttribute(const cs::ShaderAttributeID& id) = 0;
  virtual void RegisterStream(const cs::ShaderStreamID& id) = 0;

  virtual csUInt32 GetNumberOfAttributes() const = 0;
  virtual cs::iShaderAttribute* GetAttribute(const cs::ShaderAttributeID& id) = 0;
  virtual cs::iShaderAttribute* GetAttribute(const std::string& attributeName) = 0;

  virtual csUInt16 GetNumberOfStreams() const = 0;
  virtual cs::iShaderStream* GetStream(const cs::ShaderStreamID& id) = 0;
  virtual cs::iShaderStream* GetStream(const std::string& streamName) = 0;

};


CS_CLASS()
class CSE_API ShaderWrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::iShader, ShaderWrapper, cs::ResourceWrapper);
};

}