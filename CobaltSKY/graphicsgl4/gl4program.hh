#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4program.refl.hh>


class csShaderGL4;
class csShaderAttributeGL4;
class csShaderStreamGL4;


CS_CLASS()
class csProgramGL4 : public CS_SUPER(iShader)
{
  CS_CLASS_GEN_OBJECT;

public:
  csProgramGL4();
  virtual ~csProgramGL4();

  void Bind();

  virtual void RegisterAttribute(const csShaderAttributeID &id);
  virtual void RegisterStream(const csShaderStreamID &id);

  virtual csUInt32 GetNumberOfAttributes() const;
  virtual iShaderAttribute *GetAttribute(csUInt32 idx);
  virtual iShaderAttribute *GetAttribute(const csShaderAttributeID &id);
  virtual iShaderAttribute *GetAttribute(const csString &attributeName);

  virtual csUInt16 GetNumberOfStreams() const;
  virtual iShaderStream *GetStream(const csShaderStreamID &id);
  virtual iShaderStream *GetStream(const csString &streamName);


  void AttachShader(csShaderGL4 *shader);
  void DetachShader(csShaderGL4 *shader);

  bool Link();
  csString GetLinkErrorLog() const;

private:
  GLuint m_name;

  std::vector<csShaderGL4*> m_shaders;

  void InitializeSystemStreams();
  void InitializeSystemAttributes();

  void ResizeAttributes(csUInt32 id);
  void ResizeStreams(csUInt32 id);
  std::vector<csShaderAttributeGL4*> m_attributes;
  std::vector<csShaderStreamGL4*> m_streams;

  std::vector<csShaderAttributeGL4*> m_namedAttributes;
  std::vector<csShaderStreamGL4*> m_namedStreams;

};


