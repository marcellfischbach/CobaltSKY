#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shaderattribute.refl.hh>

CS_CLASS()
class csShaderAttributeGL4 : public CS_SUPER(cs::iShaderAttribute)
{
  CS_CLASS_GEN_OBJECT;

public:
  csShaderAttributeGL4();
  virtual ~csShaderAttributeGL4();

  void SetName(const std::string &name);
  virtual const std::string &GetName() const;

  virtual void SetArrayIndex(csUInt32 idx);

  virtual void Set(float x);
  virtual void Set(float x, float y);
  virtual void Set(float x, float y, float z);
  virtual void Set(float x, float y, float z, float w);

  virtual void Set(csInt32 x);
  virtual void Set(csInt32 x, csInt32 y);
  virtual void Set(csInt32 x, csInt32 y, csInt32 z);
  virtual void Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w);

  virtual void Set(const cs::Vector2f &v);
  virtual void Set(const cs::Vector3f &v);
  virtual void Set(const cs::Vector4f &v);
  virtual void Set(const cs::Color4f &v);
  virtual void Set(const cs::Vector2f *vs, csSize num);
  virtual void Set(const cs::Vector3f *vs, csSize num);
  virtual void Set(const cs::Vector4f *vs, csSize num);
  virtual void Set(const csInt32 *vs, csSize num);
  virtual void Set(const csUInt32 *vs, csSize num);
  virtual void Set(const float *vs, csSize num);

  virtual void Set(const cs::Matrix3f &m);
  virtual void Set(const cs::Matrix3f *ms, csSize num);
  virtual void Set(const cs::Matrix4f &m);
  virtual void Set(const cs::Matrix4f *ms, csSize num);


  void SetLocation(GLint location);
  GLint GetLocation() const;

  void SetValid(bool valid);
  bool IsValid()const;
private:
  std::string m_name;
  GLint m_location;
  csUInt32 m_arrayIndex;
  GLint m_absLocation;
  bool m_valid;
};