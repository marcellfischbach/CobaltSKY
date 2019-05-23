#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4program.refl.hh>


class csShaderGL4Wrapper;
class csShaderAttributeGL4;
class csShaderStreamGL4;

namespace cs
{

CS_CLASS()
class ProgramGL4 : public CS_SUPER(cs::iShader)
{
  CS_CLASS_GEN_OBJECT;

public:
  ProgramGL4();
  virtual ~ProgramGL4();

  void Bind();

  virtual void RegisterAttribute(const cs::ShaderAttributeID & id);
  virtual void RegisterStream(const cs::ShaderStreamID & id);

  virtual csUInt32 GetNumberOfAttributes() const;
  virtual cs::iShaderAttribute* GetAttribute(csUInt32 idx);
  virtual cs::iShaderAttribute* GetAttribute(const cs::ShaderAttributeID & id);
  virtual cs::iShaderAttribute* GetAttribute(const std::string & attributeName);

  virtual csUInt16 GetNumberOfStreams() const;
  virtual cs::iShaderStream* GetStream(const cs::ShaderStreamID & id);
  virtual cs::iShaderStream* GetStream(const std::string & streamName);


  void AttachShader(csShaderGL4Wrapper * shader);
  void DetachShader(csShaderGL4Wrapper * shader);

  bool Link();
  std::string GetLinkErrorLog() const;

private:
  GLuint m_name;

  std::vector<csShaderGL4Wrapper*> m_shaders;

  void InitializeSystemStreams();
  void InitializeSystemAttributes();

  void ResizeAttributes(csUInt32 id);
  void ResizeStreams(csUInt32 id);
  std::vector<csShaderAttributeGL4*> m_attributes;
  std::vector<csShaderStreamGL4*> m_streams;

  std::vector<csShaderAttributeGL4*> m_namedAttributes;
  std::vector<csShaderStreamGL4*> m_namedStreams;

};

CS_CLASS()
class ProgramGL4Wrapper : public CS_SUPER(cs::ShaderWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::ProgramGL4, ProgramGL4Wrapper, cs::ShaderWrapper);
};




}
