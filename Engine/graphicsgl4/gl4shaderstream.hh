#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/ishader.hh>
#include <cobalt/graphics/ishaderattribute.hh>
#include <cobalt/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shaderstream.refl.hh>

namespace cs
{

CS_CLASS()
class ShaderStreamGL4 : public CS_SUPER(cs::iShaderStream)
{
  CS_CLASS_GEN_OBJECT;
public:
  ShaderStreamGL4();
  virtual ~ShaderStreamGL4();

  void SetName(const std::string & name);
  virtual const std::string& GetName() const;

  void SetLocation(GLint location);
  GLint GetLocation() const;

  virtual void SetArrayIndex(csUInt32 idx);

  virtual void Set(csSize size, csSize stride, csSize offset, cs::eDataType type);
  virtual void Set(csSize size, csSize stride, const void* data, cs::eDataType type);

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


}

