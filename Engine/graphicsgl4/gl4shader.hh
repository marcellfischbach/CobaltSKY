#pragma once


#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <csrefl/class.hh>
#include <cobalt/core/csresourcewrapper.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shader.refl.hh>


namespace cs
{

CS_CLASS()
class ShaderGL4 : public cs::iObject
{
  CS_CLASS_GEN_OBJECT;
public:
  ShaderGL4();
  virtual ~ShaderGL4();

  void SetSource(const std::string& source);
  const std::string& GetSource() const;

  GLuint GetName() const;

  void SetShaderType(cs::eShaderType shaderType);
  cs::eShaderType GetShaderType() const;

  bool Compile();
  std::string GetCompileErrorLog() const;
private:
  cs::eShaderType m_shaderType;

  std::string m_shader;
  GLuint m_name;

};

CS_CLASS()
class ShaderGL4Wrapper : public CS_SUPER(cs::ResourceWrapper)
{
  CS_CLASS_GEN;
  CS_RESOURCE_WRAPPER(cs::ShaderGL4, ShaderGL4Wrapper, cs::ResourceWrapper);
};




}