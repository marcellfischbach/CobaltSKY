#pragma once


#include <RenderGL4/Export.hh>
#include <RenderGL4/Enums.hh>
#include <Valkyrie/Core/Object.hh>
#include <Valkyrie/Graphics/IShader.hh>
#include <GL/glew.h>
#include <RenderGL4/Shader.refl.hh>
#include <vector>

VK_CLASS();
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


VK_CLASS();
class vkShaderAttributeGL4 : public IShaderAttribute
{
  VK_CLASS_GEN_OBJECT;

public:
  vkShaderAttributeGL4();
  virtual ~vkShaderAttributeGL4();

  void SetName(const vkString &name);
  virtual const vkString &GetName() const;

  virtual void SetArrayIndex(vkUInt32 idx);

  virtual void Set(float x);
  virtual void Set(float x, float y);
  virtual void Set(float x, float y, float z);
  virtual void Set(float x, float y, float z, float w);

  virtual void Set(vkInt32 x);
  virtual void Set(vkInt32 x, vkInt32 y);
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z);
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z, vkInt32 w);

  virtual void Set(const vkVector2f &v);
  virtual void Set(const vkVector3f &v);
  virtual void Set(const vkVector4f &v);
  virtual void Set(const vkColor4f &v);

  virtual void Set(const vkMatrix3f &m);
  virtual void Set(const vkMatrix4f &m);


  void SetLocation(GLint location);
  GLint GetLocation() const;

private:
  vkString m_name;
  GLint m_location;
  vkUInt32 m_arrayIndex;
  GLint m_absLocation;

};


VK_CLASS();
class vkShaderStreamGL4 : public IShaderStream
{
  VK_CLASS_GEN_OBJECT;
public:
  vkShaderStreamGL4();
  virtual ~vkShaderStreamGL4();

  void SetName(const vkString &name);
  virtual const vkString &GetName() const;

  void SetLocation(GLint location);
  GLint GetLocation() const;

  virtual void SetArrayIndex(vkUInt32 idx);

  virtual void Set(vkSize size, vkSize stride, vkSize offset, vkDataType type);
  virtual void Set(vkSize size, vkSize stride, const void *data, vkDataType type);

  virtual void SetFrequency(vkUInt8 frequency);

  virtual void Enable();
  virtual void Disable();

private:
  vkString m_name;
  GLint m_location;
  vkUInt32 m_arrayIndex;
  GLint m_absLocation;
};


VK_CLASS();
class vkProgramGL4 : public IShader
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

  virtual vkUInt16 GetNumberOfStreams() const;
  virtual IShaderStream *GetStream(const vkShaderStreamID &id);

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

};


