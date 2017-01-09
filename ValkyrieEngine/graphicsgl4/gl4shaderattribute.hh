#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shaderattribute.refl.hh>

VK_CLASS()
class vkShaderAttributeGL4 : public VK_SUPER(iShaderAttribute)
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
  virtual void Set(const vkVector2f *vs, vkSize num);
  virtual void Set(const vkVector3f *vs, vkSize num);
  virtual void Set(const vkVector4f *vs, vkSize num);
  virtual void Set(const vkInt32 *vs, vkSize num);
  virtual void Set(const vkUInt32 *vs, vkSize num);
  virtual void Set(const float *vs, vkSize num);

  virtual void Set(const vkMatrix3f &m);
  virtual void Set(const vkMatrix3f *ms, vkSize num);
  virtual void Set(const vkMatrix4f &m);
  virtual void Set(const vkMatrix4f *ms, vkSize num);


  void SetLocation(GLint location);
  GLint GetLocation() const;

  void SetValid(bool valid);
  bool IsValid()const;
private:
  vkString m_name;
  GLint m_location;
  vkUInt32 m_arrayIndex;
  GLint m_absLocation;
  bool m_valid;
};