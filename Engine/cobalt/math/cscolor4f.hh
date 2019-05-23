#pragma once



#include <cobalt/csexport.hh>
#include <cobalt/csdefs.hh>
#include <math.h>

namespace cs
{

struct CSE_API Color4f
{
public:
  float r;
  float g;
  float b;
  float a;

public:
  CS_FORCEINLINE Color4f(const float* f)
    : r(f[0])
    , g(f[1])
    , b(f[2])
    , a(f[3])
  {

  }

  CS_FORCEINLINE Color4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
  {

  }

};

}