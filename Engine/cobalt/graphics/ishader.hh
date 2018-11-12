#pragma once


#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/csshaderattributeid.hh>
#include <cobalt/graphics/csshaderstreamid.hh>
#include <cobalt/csenums.hh>
#include <string>

#include <cobalt/graphics/ishader.refl.hh>

struct iShaderAttribute;
struct iShaderStream;


CS_INTERFACE();
struct CSE_API iShader : public iObject
{
  CS_CLASS_GEN;

  virtual ~iShader() { }

  virtual void RegisterAttribute(const csShaderAttributeID &id) = 0;
  virtual void RegisterStream(const csShaderStreamID &id) = 0;

  virtual csUInt32 GetNumberOfAttributes() const = 0;
  virtual iShaderAttribute *GetAttribute(const csShaderAttributeID &id) = 0;
  virtual iShaderAttribute *GetAttribute(const std::string &attributeName) = 0;

  virtual csUInt16 GetNumberOfStreams() const = 0;
  virtual iShaderStream *GetStream(const csShaderStreamID &id) = 0;
  virtual iShaderStream *GetStream(const std::string &streamName) = 0;

};


CS_CLASS()
class CSE_API csShaderWrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(iShader, csShaderWrapper, csResourceWrapper);
};
