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



namespace cs
{
class ShaderAttributeGL4;
class ShaderGL4Wrapper;
class ShaderStreamGL4;

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


  void AttachShader(cs::ShaderGL4Wrapper * shader);
  void DetachShader(cs::ShaderGL4Wrapper * shader);

  bool Link();
  std::string GetLinkErrorLog() const;

private:
  GLuint m_name;

  std::vector<cs::ShaderGL4Wrapper*> m_shaders;

  void InitializeSystemStreams();
  void InitializeSystemAttributes();

  void ResizeAttributes(csUInt32 id);
  void ResizeStreams(csUInt32 id);
  std::vector<cs::ShaderAttributeGL4*> m_attributes;
  std::vector<cs::ShaderStreamGL4*> m_streams;

  std::vector<cs::ShaderAttributeGL4*> m_namedAttributes;
  std::vector<cs::ShaderStreamGL4*> m_namedStreams;

};

CS_CLASS()
class ProgramGL4Wrapper : public CS_SUPER(cs::ShaderWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::ProgramGL4, ProgramGL4Wrapper, cs::ShaderWrapper);
};




}
