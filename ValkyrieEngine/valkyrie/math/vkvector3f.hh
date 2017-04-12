#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/vkdefs.hh>
#include <valkyrie/math/vkvector2f.hh>
#include <math.h>
#include <stdio.h>

struct VKE_API vkVector3f
{
public:
  float x;
  float y;
  float z;

public:
  VK_FORCEINLINE vkVector3f(const float *f)
    : x(f[0])
    , y(f[1])
    , z(f[2])
  {

  }

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

  VK_FORCEINLINE void Set(const vkVector3f &v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
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

  VK_FORCEINLINE vkVector3f operator*(float v) const
  {
    return vkVector3f(x*v, y*v, z*v);
  }

  VK_FORCEINLINE vkVector3f operator/(float v) const
  {
    return vkVector3f(x/v, y/v, z/v);
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


  VK_FORCEINLINE static vkVector3f &MulAdd(const vkVector3f &v0, const vkVector3f v1, float fact, vkVector3f &r)
  {
    r.x = v0.x + v1.x * fact;
    r.y = v0.y + v1.y * fact;
    r.z = v0.z + v1.z * fact;
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

  VK_FORCEINLINE vkVector3f &Normalized(vkVector3f &r) const
  {
    float l = Length();
    r.x = x / l;
    r.y = y / l;
    r.z = z / l;
    return r;
  }

  VK_FORCEINLINE static vkVector3f &Cross(const vkVector3f &v0, const vkVector3f &v1, vkVector3f &r)
  {
    r.Set(v0.y*v1.z - v0.z*v1.y, v0.z*v1.x - v0.x*v1.z, v0.x*v1.y - v0.y*v1.x);
    return r;
  }

  VK_FORCEINLINE void Debug(const char *message)
  {
    printf("vkVector3f ");
    if (message)
    {
      printf("%s ", message);
    }
    printf("<%.2f %.2f %.2f>\n", x, y, z);
  }
};

