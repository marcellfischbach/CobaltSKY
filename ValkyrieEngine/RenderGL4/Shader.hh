#pragma once


#include <RenderGL4/Export.hh>
#include <RenderGL4/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <GL/glew.h>
#include <RenderGL4/Shader.refl.hh>


VK_CLASS();
class VKGL4_API vkShader : public IObject
{
  VK_CLASS_GEN_OBJECT;
public:
  vkShader();
  virtual ~vkShader();

  void SetSource(const vkString &source);
  const vkString &GetSource() const;

  GLuint GetName() const;

  void SetShaderType(vkShaderType shaderType);
  vkShaderType GetShaderType() const;

  bool Compile();
  vkString GetCompileErrorLog() const;
private:
  vkShaderType m_shaderType;

  vkString m_shader;
  GLuint m_name;

};