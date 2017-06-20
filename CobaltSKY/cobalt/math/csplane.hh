#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csvector3f.hh>

struct CSE_API csPlane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  CS_FORCEINLINE csPlane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , d(d)
  {

  }

  CS_FORCEINLINE csPlane(const csVector3f &pos, const csVector3f &norm)
  {
    csVector3f nn;
    norm.Normalized(nn);
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  CS_FORCEINLINE float Distance(const csVector3f &pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  CS_FORCEINLINE csVector3f& ReflectPoint(const csVector3f &p, csVector3f &r) const
  {
    csVector3f origin, diff;
    csVector3f normal = csVector3f(x, y, z);
    csVector3f::Mul(normal, -d, origin);
    csVector3f::Sub(p, origin, diff);

    csVector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
    csVector3f::Sub(diff, r, r);
    return csVector3f::Add(r, origin, r);
  }

  csVector3f &ReflectDirection(const csVector3f &d, csVector3f &r) const
  {
    csVector3f normal = csVector3f(x, y, z);
    csVector3f::Mul(normal, 2.0f * d.Dot(normal), r);
    return csVector3f::Sub(d, r, r);
  }


}; 