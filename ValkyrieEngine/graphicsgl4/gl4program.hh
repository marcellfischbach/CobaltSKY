#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4program.refl.hh>


class vkShaderGL4;
class vkShaderAttributeGL4;
class vkShaderStreamGL4;


VK_CLASS()
class vkProgramGL4 : public VK_SUPER(IShader)
{
  VK_CLASS_GEN_OBJECT;

public:
  vkProgramGL4();
  virtual ~vkProgramGL4();

  void Bind();

  virtual void RegisterAttribute(const vkShaderAttributeID &id);
  virtual void RegisterStream(const vkShaderStreamID &id);

  virtual vkUInt32 GetNumberOfAttributes() const;
  virtual IShaderAttribute *GetAttribute(vkUInt32 idx);
  virtual IShaderAttribute *GetAttribute(const vkShaderAttributeID &id);
  virtual IShaderAttribute *GetAttribute(const vkString &attributeName);

  virtual vkUInt16 GetNumberOfStreams() const;
  virtual IShaderStream *GetStream(const vkShaderStreamID &id);
  virtual IShaderStream *GetStream(const vkString &streamName);


  void AttachShader(vkShaderGL4 *shader);
  void DetachShader(vkShaderGL4 *shader);

  bool Link();
  vkString GetLinkErrorLog() const;

private:
  GLuint m_name;

  std::vector<vkShaderGL4*> m_shaders;

  void InitializeSystemStreams();
  void InitializeSystemAttributes();

  void ResizeAttributes(vkUInt32 id);
  void ResizeStreams(vkUInt32 id);
  std::vector<vkShaderAttributeGL4*> m_attributes;
  std::vector<vkShaderStreamGL4*> m_streams;

  std::vector<vkShaderAttributeGL4*> m_namedAttributes;
  std::vector<vkShaderStreamGL4*> m_namedStreams;

};


