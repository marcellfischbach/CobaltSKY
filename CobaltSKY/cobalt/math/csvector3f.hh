#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/math/csvector2f.hh>
#include <math.h>
#include <stdio.h>

struct CSE_API csVector3f
{
public:
  float x;
  float y;
  float z;

public:
  CS_FORCEINLINE csVector3f(const float *f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
  {

  }

  CS_FORCEINLINE csVector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    : x(x)
    , y(y)
    , z(z)
  {

  }

  CS_FORCEINLINE csVector3f(const csVector2f& v, float z = 0.0f)
    : x(v.x)
    , y(v.y)
    , z(z)
  {

  }

  CS_FORCEINLINE void Set(float x, float y, float z)
  {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  CS_FORCEINLINE void Set(const csVector3f &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
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

  CS_FORCEINLINE csVector3f &operator+=(const csVector3f &v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator-=(const csVector3f &v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator*=(const csVector3f &v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator/=(const csVector3f &v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  CS_FORCEINLINE csVector3f &operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  CS_FORCEINLINE csVector3f operator*(float v) const
  {
    return csVector3f(x*v, y*v, z*v);
  }

  CS_FORCEINLINE csVector3f operator/(float v) const
  {
    return csVector3f(x/v, y/v, z/v);
  }

  CS_FORCEINLINE static csVector3f &Add(const csVector3f &v0, const csVector3f &v1, csVector3f &r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Add(const csVector3f &v0, float v1, csVector3f &r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Sub(const csVector3f &v0, const csVector3f &v1, csVector3f &r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Sub(const csVector3f &v0, float v1, csVector3f &r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    return r;
  }
  CS_FORCEINLINE static csVector3f &Mul(const csVector3f &v0, const csVector3f &v1, csVector3f &r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Mul(const csVector3f &v0, float v1, csVector3f &r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    return r;
  }


  CS_FORCEINLINE static csVector3f &MulAdd(const csVector3f &v0, const csVector3f v1, float fact, csVector3f &r)
  {
    r.x = v0.x + v1.x * fact;
    r.y = v0.y + v1.y * fact;
    r.z = v0.z + v1.z * fact;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Div(const csVector3f &v0, const csVector3f &v1, csVector3f &r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Div(const csVector3f &v0, float v1, csVector3f &r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    return r;
  }

  CS_FORCEINLINE float Dot() const
  {
    return x*x + y*y + z*z;
  }

  CS_FORCEINLINE float Dot(const csVector3f &v) const
  {
    return x*v.x + y*v.y + z*v.z;
  }

  CS_FORCEINLINE static float Dot(const csVector3f &v0, const csVector3f &v1)
  {
    return v0.x*v1.x + v0.y*v1.y + v0.z*v1.z;
  }

  CS_FORCEINLINE float Length() const
  {
    return (float)sqrt(x*x + y*y + z*z);
  }

  CS_FORCEINLINE csVector3f &Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  CS_FORCEINLINE csVector3f &Normalized(csVector3f &r) const
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }

  CS_FORCEINLINE static csVector3f &Cross(const csVector3f &v0, const csVector3f &v1, csVector3f &r)
  {
    r.Set(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
    return r;
  }

  CS_FORCEINLINE void Debug(const char *message)
  {
    printf("csVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%.2f %.2f %.2f>\n", x, y, z);
  }
};

