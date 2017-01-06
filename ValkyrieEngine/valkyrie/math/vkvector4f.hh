#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <valkyrie/math/vkvector3f.hh>
#include <math.h>
#include <stdio.h>


struct VKE_API vkVector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  VK_FORCEINLINE vkVector4f(const float *f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
    , w(f[3])
  {

  }

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


  VK_FORCEINLINE bool operator== (const vkVector4f &o) const
  {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  VK_FORCEINLINE bool operator!= (const vkVector4f &o) const
  {
    return !(x == o.x && y == o.y && z == o.z && w == o.w);
  }


  VK_FORCEINLINE void Debug(const char *message)
  {
    printf("vkVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%f %f %f %f>\n", x, y, z, w);
  }
};
