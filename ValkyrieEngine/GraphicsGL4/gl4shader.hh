#pragma once


#include <GraphicsGL4/gl4export.hh>
#include <GraphicsGL4/gl4enums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <GraphicsGL4/gl4shader.refl.hh>

VK_CLASS()
class vkShaderGL4 : public IObject
{
  VK_CLASS_GEN_OBJECT;
public:
  vkShaderGL4();
  virtual ~vkShaderGL4();

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


