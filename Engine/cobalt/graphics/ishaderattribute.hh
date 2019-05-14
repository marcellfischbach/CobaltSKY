#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/cstypes.hh>
#include <cobalt/math/cscolor4f.hh>
#include <cobalt/math/csvector.hh>
#include <cobalt/math/csmatrix.hh>

#include <cobalt/graphics/ishaderattribute.refl.hh>

CS_CLASS();
struct CSE_API iShaderAttribute : public cs::iObject
{
  CS_CLASS_GEN;

  virtual ~iShaderAttribute() {}

  virtual const std::string &GetName() const = 0;

  virtual void SetArrayIndex(csUInt32 idx) = 0;

  virtual void Set(float x) = 0;
  virtual void Set(float x, float y) = 0;
  virtual void Set(float x, float y, float z) = 0;
  virtual void Set(float x, float y, float z, float w) = 0;

  virtual void Set(csInt32 x) = 0;
  virtual void Set(csInt32 x, csInt32 y) = 0;
  virtual void Set(csInt32 x, csInt32 y, csInt32 z) = 0;
  virtual void Set(csInt32 x, csInt32 y, csInt32 z, csInt32 w) = 0;

  virtual void Set(const csVector2f &v) = 0;
  virtual void Set(const csVector3f &v) = 0;
  virtual void Set(const csVector4f &v) = 0;
  virtual void Set(const csColor4f &v) = 0;
  virtual void Set(const csVector2f *v, csSize num) = 0;
  virtual void Set(const csVector3f *v, csSize num) = 0;
  virtual void Set(const csVector4f *v, csSize num) = 0;
  virtual void Set(const csInt32 *vs, csSize num) = 0;
  virtual void Set(const csUInt32 *vs, csSize num) = 0;
  virtual void Set(const float *vs, csSize num) = 0;


  virtual void Set(const csMatrix3f &m) = 0;
  virtual void Set(const csMatrix3f *ms, csSize num) = 0;
  virtual void Set(const csMatrix4f &m) = 0;
  virtual void Set(const csMatrix4f *ms, csSize num) = 0;


};

