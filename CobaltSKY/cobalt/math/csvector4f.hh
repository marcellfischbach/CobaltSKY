#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/math/csvector2f.hh>
#include <cobalt/math/csvector3f.hh>
#include <math.h>
#include <stdio.h>


struct CSE_API csVector4f
{
public:
  float x;
  float y;
  float z;
  float w;

public:
  CS_FORCEINLINE csVector4f(const float *f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
    , w(f[3])
  {

  }

  CS_FORCEINLINE csVector4f(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , w(w)
  {

  }

  CS_FORCEINLINE csVector4f(const csVector3f &v, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(v.z)
    , w(w)
  {

  }

  CS_FORCEINLINE csVector4f(const csVector2f &v, float z = 0.0f, float w = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
    , w(w)
  {

  }

  CS_FORCEINLINE csVector4f(const csVector2f &v0, const csVector2f &v1)
    : x(v0.x)
    , y(v0.y)
    , z(v1.x)
    , w(v1.y)
  {

  }


  CS_FORCEINLINE csVector2f AsVector2f() const
  {
    return csVector2f(x, y);
  }

  CS_FORCEINLINE csVector2f& AsVector2f(csVector2f &v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  CS_FORCEINLINE csVector2f AsVector2fHigh() const
  {
    return csVector2f(z, w);
  }

  CS_FORCEINLINE csVector2f& AsVector2fHigh(csVector2f &v) const
  {
    v.x = z;
    v.y = w;
    return v;
  }


  CS_FORCEINLINE csVector3f AsVector3f() const
  {
    return csVector3f(x, y, z);
  }

  CS_FORCEINLINE csVector3f& AsVector3f(csVector3f &v) const
  {
    v.x = x;
    v.y = y;
    v.z = z;
    return v;
  }



  CS_FORCEINLINE csVector4f &operator+=(const csVector4f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    w += v;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator-=(const csVector4f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator*=(const csVector4f &v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator/=(const csVector4f &v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
    return *this;
  }

  CS_FORCEINLINE csVector4f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
    return *this;
  }

  CS_FORCEINLINE static csVector4f &Add(const csVector4f &v0, const csVector4f &v1, csVector4f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    r.w = v0.w + v1.w;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Add(const csVector4f &v0, float v1, csVector4f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    r.w = v0.w + v1;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Sub(const csVector4f &v0, const csVector4f &v1, csVector4f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    r.w = v0.w - v1.w;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Sub(const csVector4f &v0, float v1, csVector4f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    r.w = v0.w - v1;
    return r;
  }
  CS_FORCEINLINE static csVector4f &Mul(const csVector4f &v0, const csVector4f &v1, csVector4f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    r.w = v0.w * v1.w;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Mul(const csVector4f &v0, float v1, csVector4f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    r.w = v0.w * v1;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Div(const csVector4f &v0, const csVector4f &v1, csVector4f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    r.w = v0.w / v1.w;
    return r;
  }

  CS_FORCEINLINE static csVector4f &Div(const csVector4f &v0, float v1, csVector4f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    r.w = v0.w / v1;
    return r;
  }

  CS_FORCEINLINE float Dot() const
  {
    return x*x + y*y + z*z + w*w;
  }

  CS_FORCEINLINE float Dot(const csVector4f &v) const
  {
    return x*v.x + y*v.y + z*v.z + w*v.w;
  }

  CS_FORCEINLINE static float Dot(const csVector4f &v0, const csVector4f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z + v0.w*v1.w;
  }

  CS_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y + z*z + w*w);
  }

  CS_FORCEINLINE csVector4f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
    return *this;
  }

  CS_FORCEINLINE csVector4f &Normalized(csVector4f &r)
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    r.w = w / l;
    return r;
  }


  CS_FORCEINLINE bool operator== (const csVector4f &o) const
  {
    return x == o.x && y == o.y && z == o.z && w == o.w;
  }

  CS_FORCEINLINE bool operator!= (const csVector4f &o) const
  {
    return !(x == o.x && y == o.y && z == o.z && w == o.w);
  }


  CS_FORCEINLINE void Debug(const char *message)
  {
    printf("csVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%f %f %f %f>\n", x, y, z, w);
  }
};