#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/math/csmatrix.hh>

#include <cobalt/graphics/ishaderattribute.refl.hh>

namespace cs
{

CS_CLASS();
struct CSE_API iShaderAttribute : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iShaderAttribute() {}

  virtual const std::string& GetName() const = 0;

  virtual void SetArrayIndex(csUInt32 idx) = 0;

  virtual void Set(float x) = 0;
  virtual void Set(float x, float y) = 0;
  virtual void Set(float x, float y, float z) = 0;
  virtual void Set(float x, float y, float z, float w) = 0;

  virtual void Set(csInt32 x) = 0;
  virtual void Set(csInt32 x, csInt32 y) = 0;
  virtual void Set(csInt32 x, csInt32 y, csInt32 z) = 0;
  virtual void Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w) = 0;

  virtual void Set(const cs::Vector2f& v) = 0;
  virtual void Set(const cs::Vector3f& v) = 0;
  virtual void Set(const cs::Vector4f& v) = 0;
  virtual void Set(const cs::Color4f& v) = 0;
  virtual void Set(const cs::Vector2f* v, csSize num) = 0;
  virtual void Set(const cs::Vector3f* v, csSize num) = 0;
  virtual void Set(const cs::Vector4f* v, csSize num) = 0;
  virtual void Set(const csInt32* vs, csSize num) = 0;
  virtual void Set(const csUInt32* vs, csSize num) = 0;
  virtual void Set(const float* vs, csSize num) = 0;


  virtual void Set(const cs::Matrix3f& m) = 0;
  virtual void Set(const cs::Matrix3f* ms, csSize num) = 0;
  virtual void Set(const cs::Matrix4f& m) = 0;
  virtual void Set(const cs::Matrix4f* ms, csSize num) = 0;


};

}

