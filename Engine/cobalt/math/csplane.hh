#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csvector3f.hh>

namespace cs
{

struct CSE_API Plane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  CS_FORCEINLINE Plane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , d(d)
  {

  }

  CS_FORCEINLINE Plane(const cs::Vector3f& pos, const cs::Vector3f& norm)
  {
    cs::Vector3f nn;
    norm.Normalized(nn);
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE float Distance(const cs::Vector3f& pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  CS_FORCEINLINE cs::Vector3f& ReflectPoint(const cs::Vector3f& p, cs::Vector3f& r) const
  {
    cs::Vector3f origin, diff;
    cs::Vector3f normal = cs::Vector3f(x, y, z);
    cs::Vector3f::Mul(normal, -d, origin);
    cs::Vector3f::Sub(p, origin, diff);

    cs::Vector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
    cs::Vector3f::Sub(diff, r, r);
    return cs::Vector3f::Add(r, origin, r);
  }

  cs::Vector3f& ReflectDirection(const cs::Vector3f& d, cs::Vector3f& r) const
  {
    cs::Vector3f normal = cs::Vector3f(x, y, z);
    cs::Vector3f::Mul(normal, 2.0f * d.Dot(normal), r);
    return cs::Vector3f::Sub(d, r, r);
  }


};

}

