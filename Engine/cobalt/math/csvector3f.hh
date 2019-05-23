#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <cobalt/math/csvector2f.hh>
#include <math.h>
#include <stdio.h>

namespace cs
{

struct CSE_API Vector3f
{
public:
  float x;
  float y;
  float z;

public:
  CS_FORCEINLINE Vector3f(const float* f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
  {

  }

  CS_FORCEINLINE Vector3f(float x = 0.0f, float y = 0.0f, float z = 0.0f)
    : x(x)
    , y(y)
    , z(z)
  {

  }

  CS_FORCEINLINE Vector3f(const cs::Vector2f& v, float z = 0.0f)
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

  CS_FORCEINLINE void Set(const cs::Vector3f& v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }

  CS_FORCEINLINE cs::Vector2f AsVector2f() const
  {
    return cs::Vector2f(x, y);
  }

  CS_FORCEINLINE cs::Vector2f& AsVector2f(cs::Vector2f& v) const
  {
    v.x = x;
    v.y = y;
    return v;
  }

  CS_FORCEINLINE cs::Vector3f& operator+=(const cs::Vector3f& v)
  {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator+=(float v)
  {
    x += v;
    y += v;
    z += v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator-=(const cs::Vector3f& v)
  {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator-=(float v)
  {
    x -= v;
    y -= v;
    z -= v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator*=(const cs::Vector3f& v)
  {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator*=(float v)
  {
    x *= v;
    y *= v;
    z *= v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator/=(const cs::Vector3f& v)
  {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& operator/=(float v)
  {
    x /= v;
    y /= v;
    z /= v;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f operator*(float v) const
  {
    return cs::Vector3f(x * v, y * v, z * v);
  }

  CS_FORCEINLINE cs::Vector3f operator/(float v) const
  {
    return cs::Vector3f(x / v, y / v, z / v);
  }

  CS_FORCEINLINE cs::Vector3f operator+(const cs::Vector3f & o) const
  {
    return cs::Vector3f(x + o.x, y + o.y, z + o.z);
  }

  CS_FORCEINLINE cs::Vector3f operator-(const cs::Vector3f & o) const
  {
    return cs::Vector3f(x - o.x, y - o.y, z - o.z);
  }

  CS_FORCEINLINE static cs::Vector3f& Add(const cs::Vector3f & v0, const cs::Vector3f & v1, cs::Vector3f & r)
  {
    r.x = v0.x + v1.x;
    r.y = v0.y + v1.y;
    r.z = v0.z + v1.z;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Add(const cs::Vector3f & v0, float v1, cs::Vector3f & r)
  {
    r.x = v0.x + v1;
    r.y = v0.y + v1;
    r.z = v0.z + v1;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Sub(const cs::Vector3f & v0, const cs::Vector3f & v1, cs::Vector3f & r)
  {
    r.x = v0.x - v1.x;
    r.y = v0.y - v1.y;
    r.z = v0.z - v1.z;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Sub(const cs::Vector3f & v0, float v1, cs::Vector3f & r)
  {
    r.x = v0.x - v1;
    r.y = v0.y - v1;
    r.z = v0.z - v1;
    return r;
  }
  CS_FORCEINLINE static cs::Vector3f& Mul(const cs::Vector3f & v0, const cs::Vector3f & v1, cs::Vector3f & r)
  {
    r.x = v0.x * v1.x;
    r.y = v0.y * v1.y;
    r.z = v0.z * v1.z;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Mul(const cs::Vector3f & v0, float v1, cs::Vector3f & r)
  {
    r.x = v0.x * v1;
    r.y = v0.y * v1;
    r.z = v0.z * v1;
    return r;
  }


  CS_FORCEINLINE static cs::Vector3f& MulAdd(const cs::Vector3f & v0, const cs::Vector3f v1, float fact, cs::Vector3f & r)
  {
    r.x = v0.x + v1.x * fact;
    r.y = v0.y + v1.y * fact;
    r.z = v0.z + v1.z * fact;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Div(const cs::Vector3f & v0, const cs::Vector3f & v1, cs::Vector3f & r)
  {
    r.x = v0.x / v1.x;
    r.y = v0.y / v1.y;
    r.z = v0.z / v1.z;
    return r;
  }

  CS_FORCEINLINE static cs::Vector3f& Div(const cs::Vector3f & v0, float v1, cs::Vector3f & r)
  {
    r.x = v0.x / v1;
    r.y = v0.y / v1;
    r.z = v0.z / v1;
    return r;
  }

  CS_FORCEINLINE float Dot() const
  {
    return x * x + y * y + z * z;
  }

  CS_FORCEINLINE float Dot(const cs::Vector3f & v) const
  {
    return x * v.x + y * v.y + z * v.z;
  }

  CS_FORCEINLINE static float Dot(const cs::Vector3f & v0, const cs::Vector3f & v1)
  {
    return v0.x* v1.x + v0.y * v1.y + v0.z * v1.z;
  }

  CS_FORCEINLINE float Length() const
  {
    return (float)sqrt(x * x + y * y + z * z);
  }

  CS_FORCEINLINE cs::Vector3f& Normalize()
  {
    float l = Length();
    x /= l;
    y /= l;
    z /= l;
    return *this;
  }

  CS_FORCEINLINE cs::Vector3f& Normalized(cs::Vector3f & r) const
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }


  CS_FORCEINLINE static cs::Vector3f& Cross(const cs::Vector3f & v0, const cs::Vector3f & v1, cs::Vector3f & r)
  {
    r.Set(v0.y * v1.z - v0.z * v1.y, v0.z * v1.x - v0.x * v1.z, v0.x * v1.y - v0.y * v1.x);
    return r;
  }

  CS_FORCEINLINE void Debug(const char* message)
  {
    printf("cs::Vector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%.2f %.2f %.2f>\n", x, y, z);
  }
};


}