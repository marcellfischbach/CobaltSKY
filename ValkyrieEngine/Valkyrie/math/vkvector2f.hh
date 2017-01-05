#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/vkdefs.hh>
#include <math.h>
#include <stdio.h>


struct VKE_API vkVector2f
{
public:
  float x;
  float y;

public:
  VK_FORCEINLINE vkVector2f(const float *f)
    : x(f[0])
    , y(f[1])
  {

  }

  VK_FORCEINLINE vkVector2f(float x = 0.0f, float y = 0.0f)
    : x(x)
    , y(y)
  {

  }

  VK_FORCEINLINE void Set(float x, float y)
  {
    this->x = x;
    this->y = y;
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


