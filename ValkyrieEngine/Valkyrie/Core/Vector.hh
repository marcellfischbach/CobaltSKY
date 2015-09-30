#pragma once

#include <Valkyrie/Export.hh>
#include <Valkyrie/Defs.hh>
#include <math.h>



struct VKE_API vkVector2f
{
public:
  float x;
  float y;

public:
  VK_FORCEINLINE vkVector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  VK_FORCEINLINE vkVector2f &operator+=(const vkVector2f &v)
  {
    x += v.x;
    y += v.y;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator+=(float v)
  {
    x += v;
    y += v;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator-=(const vkVector2f &v)
  {
    x -= v.x;
    y -= v.y;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator-=(float v)
  {
    x -= v;
    y -= v;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator*=(const vkVector2f &v)
  {
    x *= v.x;
    y *= v.y;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator*=(float v)
  {
    x *= v;
    y *= v;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator/=(const vkVector2f &v)
  {
    x /= v.x;
    y /= v.y;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &operator/=(float v)
  {
    x /= v;
    y /= v;
    return *this;
  }

  VK_FORCEINLINE static vkVector2f &Add(const vkVector2f &v0, const vkVector2f &v1, vkVector2f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    return r;
  }


  VK_FORCEINLINE static vkVector2f &Add(const vkVector2f &v0, float v1, vkVector2f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    return r;
  }


  VK_FORCEINLINE static vkVector2f &Sub(const vkVector2f &v0, const vkVector2f &v1, vkVector2f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    return r;
  }


  VK_FORCEINLINE static vkVector2f &Sub(const vkVector2f &v0, float v1, vkVector2f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    return r;
  }

  VK_FORCEINLINE static vkVector2f &Mul(const vkVector2f &v0, const vkVector2f &v1, vkVector2f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    return r;
  }

  VK_FORCEINLINE static vkVector2f &Mul(const vkVector2f &v0, float v1, vkVector2f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    return r;
  }

  VK_FORCEINLINE static vkVector2f &Div(const vkVector2f &v0, const vkVector2f &v1, vkVector2f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    return r;
  }

  VK_FORCEINLINE static vkVector2f &Div(const vkVector2f &v0, float v1, vkVector2f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    return r;
  }

  VK_FORCEINLINE float Dot() const
  {
    return x*x + y*y;
  }

  VK_FORCEINLINE float Dot(const vkVector2f &v) const
  {
    return x*v.x + y*v.y;
  }

  VK_FORCEINLINE static float Dot(const vkVector2f &v0, const vkVector2f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y;
  }

  VK_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y);
  }

  VK_FORCEINLINE vkVector2f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    return *this;
  }

  VK_FORCEINLINE vkVector2f &Normalized(vkVector2f &r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    return r;
  }

};



struct VKE_API vkVector3f
{
public:
  float x;
  float y;
  float z;

public:
  VK_FORCEINLINE vkVector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    : x(x)
    , y(y)
    , z(z)
  {

  }

  VK_FORCEINLINE vkVector3f(const vkVector2f& v, float z = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
  {

  }

  VK_FORCEINLINE void Set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }


  VK_FORCEINLINE vkVector2f AsVector2f() const
  {
    return vkVector2f(x, y);
  }

  VK_FORCEINLINE vkVector2f& AsVector2f(vkVector2f &v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  VK_FORCEINLINE vkVector3f &operator+=(const vkVector3f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator-=(const vkVector3f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator*=(const vkVector3f &v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator/=(const vkVector3f &v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  VK_FORCEINLINE static vkVector3f &Add(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Add(const vkVector3f &v0, float v1, vkVector3f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Sub(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Sub(const vkVector3f &v0, float v1, vkVector3f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    return r;
  }
  VK_FORCEINLINE static vkVector3f &Mul(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Mul(const vkVector3f &v0, float v1, vkVector3f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Div(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Div(const vkVector3f &v0, float v1, vkVector3f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    return r;
  }

  VK_FORCEINLINE float Dot() const
  {
    return x*x + y*y + z*z;
  }

  VK_FORCEINLINE float Dot(const vkVector3f &v) const
  {
    return x*v.x + y*v.y + z*v.z;
  }

  VK_FORCEINLINE static float Dot(const vkVector3f &v0, const vkVector3f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
  }

  VK_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y + z*z);
  }

  VK_FORCEINLINE vkVector3f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  VK_FORCEINLINE vkVector3f &Normalized(vkVector3f &r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }

  VK_FORCEINLINE vkVector3f &Cross(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.Set(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
    return r;
  }

};



struct VKE_API vkVector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:

  VK_FORCEINLINE vkVector4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
  {

  }

  VK_FORCEINLINE vkVector4f(const vkVector3f &v, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
  {

  }

  VK_FORCEINLINE vkVector4f(const vkVector2f &v, float z = 0.0f, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
  {

  }

  VK_FORCEINLINE vkVector4f(const vkVector2f &v0, const vkVector2f &v1)
    : x(v0.x)
    , y(v0.y)
    , z(v1.x)
    , w(v1.y)
  {

  }


  VK_FORCEINLINE vkVector2f AsVector2f() const
  {
    return vkVector2f(x, y);
  }

  VK_FORCEINLINE vkVector2f& AsVector2f(vkVector2f &v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  VK_FORCEINLINE vkVector2f AsVector2fHigh() const
  {
    return vkVector2f(z, w);
  }

  VK_FORCEINLINE vkVector2f& AsVector2fHigh(vkVector2f &v) const
  {
    v.x = z;
    v.y = w;
    return v;
  }


  VK_FORCEINLINE vkVector3f AsVector3f() const
  {
    return vkVector3f(x, y, z);
  }

  VK_FORCEINLINE vkVector3f& AsVector3f(vkVector3f &v) const
  {
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
  }



  VK_FORCEINLINE vkVector4f &operator+=(const vkVector4f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator-=(const vkVector4f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator*=(const vkVector4f &v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator/=(const vkVector4f &v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  VK_FORCEINLINE static vkVector4f &Add(const vkVector4f &v0, const vkVector4f &v1, vkVector4f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    r.w = v0.w + v1.w;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Add(const vkVector4f &v0, float v1, vkVector4f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    r.w = v0.w + v1;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Sub(const vkVector4f &v0, const vkVector4f &v1, vkVector4f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    r.w = v0.w - v1.w;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Sub(const vkVector4f &v0, float v1, vkVector4f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    r.w = v0.w - v1;
    return r;
  }
  VK_FORCEINLINE static vkVector4f &Mul(const vkVector4f &v0, const vkVector4f &v1, vkVector4f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    r.w = v0.w * v1.w;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Mul(const vkVector4f &v0, float v1, vkVector4f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    r.w = v0.w * v1;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Div(const vkVector4f &v0, const vkVector4f &v1, vkVector4f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    r.w = v0.w / v1.w;
    return r;
  }

  VK_FORCEINLINE static vkVector4f &Div(const vkVector4f &v0, float v1, vkVector4f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    r.w = v0.w / v1;
    return r;
  }

  VK_FORCEINLINE float Dot() const
  {
    return x*x + y*y + z*z + w*w;
  }

  VK_FORCEINLINE float Dot(const vkVector4f &v) const
  {
    return x*v.x + y*v.y + z*v.z + w*v.w;
  }

  VK_FORCEINLINE static float Dot(const vkVector4f &v0, const vkVector4f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
  }

  VK_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y + z*z + w*w);
  }

  VK_FORCEINLINE vkVector4f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  VK_FORCEINLINE vkVector4f &Normalized(vkVector4f &r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    r.w = w / l;
    return r;
  }



};