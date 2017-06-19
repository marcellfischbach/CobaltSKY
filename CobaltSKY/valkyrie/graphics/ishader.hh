#pragma once


#include <valkyrie/csexport.hh>
#include <valkyrie/core/csclass.hh>
#include <valkyrie/core/csstring.hh>
#include <valkyrie/graphics/csshaderattributeid.hh>
#include <valkyrie/graphics/csshaderstreamid.hh>
#include <valkyrie/csenums.hh>
#include <valkyrie/graphics/ishader.refl.hh>

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
  virtual iShaderAttribute *GetAttribute(const csString &attributeName) = 0;

  virtual csUInt16 GetNumberOfStreams() const = 0;
  virtual iShaderStream *GetStream(const csShaderStreamID &id) = 0;
  virtual iShaderStream *GetStream(const csString &streamName) = 0;

};

