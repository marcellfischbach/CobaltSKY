#pragma once



#include <Valkyrie/Export.hh>
#include <Valkyrie/Defs.hh>
#include <math.h>



struct VKE_API vkColor4f
{
public:
  float r;
  float g;
  float b;
  float a;

public:
  VK_FORCEINLINE vkColor4f(const float *f)
    : r(f[0])
    , g(f[1])
    , b(f[2])
    , a(f[3])
  {

  }

  VK_FORCEINLINE vkColor4f(float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f)
    : r(r)
    , g(g)
    , b(b)
    , a(a)
  {

  }

};
