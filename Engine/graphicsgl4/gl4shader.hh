#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shader.refl.hh>

CS_CLASS()
class csShaderGL4 : public iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  csShaderGL4();
  virtual ~csShaderGL4();

  void SetSource(const std::string &source);
  const std::string &GetSource() const;

  GLuint GetName() const;

  void SetShaderType(csShaderType shaderType);
  csShaderType GetShaderType() const;

  bool Compile();
  std::string GetCompileErrorLog() const;
private:
  csShaderType m_shaderType;

  std::string m_shader;
  GLuint m_name;

};

CS_CLASS()
class csShaderGL4Wrapper : public CS_SUPER(csResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(csShaderGL4, csShaderGL4Wrapper, csResourceWrapper);
};



