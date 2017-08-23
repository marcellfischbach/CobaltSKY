#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shaderstream.refl.hh>

CS_CLASS()
class csShaderStreamGL4 : public CS_SUPER(iShaderStream)
{
  CS_CLASS_GEN_OBJECT;
public:
  csShaderStreamGL4();
  virtual ~csShaderStreamGL4();

  void SetName(const std::string &name);
  virtual const std::string &GetName() const;

  void SetLocation(GLint location);
  GLint GetLocation() const;

  virtual void SetArrayIndex(csUInt32 idx);

  virtual void Set(csSize size, csSize stride, csSize offset, csDataType type);
  virtual void Set(csSize size, csSize stride, const void *data, csDataType type);

  virtual void SetFrequency(csUInt8 frequency);

  virtual void Enable();
  virtual void Disable();

  void SetValid(bool valid);
  bool IsValid()const;
private:
  std::string m_name;
  GLint m_location;
  csUInt32 m_arrayIndex;
  GLint m_absLocation;
  bool m_valid;
};

