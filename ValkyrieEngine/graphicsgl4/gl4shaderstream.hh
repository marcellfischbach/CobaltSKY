#pragma once
#include <graphicsgl4/gl4export.hh>
#include <graphicsgl4/gl4enums.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/graphics/ishader.hh>
#include <valkyrie/graphics/ishaderattribute.hh>
#include <valkyrie/graphics/ishaderstream.hh>
#include <GL/glew.h>
#include <vector>
#include <graphicsgl4/gl4shaderstream.refl.hh>

VK_CLASS()
class vkShaderStreamGL4 : public VK_SUPER(IShaderStream)
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

  void SetValid(bool valid);
  bool IsValid()const;
private:
  vkString m_name;
  GLint m_location;
  vkUInt32 m_arrayIndex;
  GLint m_absLocation;
  bool m_valid;
};

