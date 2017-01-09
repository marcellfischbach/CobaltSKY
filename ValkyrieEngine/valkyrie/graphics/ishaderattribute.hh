#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/core/vkclass.hh>
#include <valkyrie/vktypes.hh>
#include <valkyrie/math/vkcolor4f.hh>
#include <valkyrie/math/vkvector.hh>
#include <valkyrie/math/vkmatrix.hh>

#include <valkyrie/graphics/ishaderattribute.refl.hh>

VK_INTERFACE();
struct VKE_API iShaderAttribute : public iObject
{
  VK_CLASS_GEN;

  virtual ~iShaderAttribute() {}

  virtual const vkString &GetName() const = 0;

  virtual void SetArrayIndex(vkUInt32 idx) = 0;

  virtual void Set(float x) = 0;
  virtual void Set(float x, float y) = 0;
  virtual void Set(float x, float y, float z) = 0;
  virtual void Set(float x, float y, float z, float w) = 0;

  virtual void Set(vkInt32 x) = 0;
  virtual void Set(vkInt32 x, vkInt32 y) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z) = 0;
  virtual void Set(vkInt32 x, vkInt32 y, vkInt32 z, vkInt32 w) = 0;

  virtual void Set(const vkVector2f &v) = 0;
  virtual void Set(const vkVector3f &v) = 0;
  virtual void Set(const vkVector4f &v) = 0;
  virtual void Set(const vkColor4f &v) = 0;
  virtual void Set(const vkVector2f *v, vkSize num) = 0;
  virtual void Set(const vkVector3f *v, vkSize num) = 0;
  virtual void Set(const vkVector4f *v, vkSize num) = 0;
  virtual void Set(const vkInt32 *vs, vkSize num) = 0;
  virtual void Set(const vkUInt32 *vs, vkSize num) = 0;
  virtual void Set(const float *vs, vkSize num) = 0;


  virtual void Set(const vkMatrix3f &m) = 0;
  virtual void Set(const vkMatrix3f *ms, vkSize num) = 0;
  virtual void Set(const vkMatrix4f &m) = 0;
  virtual void Set(const vkMatrix4f *ms, vkSize num) = 0;


};

