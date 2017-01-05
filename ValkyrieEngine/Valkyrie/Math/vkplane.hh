#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Math/vkvector3f.hh>

struct VKE_API vkPlane
{
public:
  float x;
  float y;
  float z;
  float d;

public:
  VK_FORCEINLINE vkPlane(float x = 0.0f, float y = 0.0f, float z = 0.0f, float d = 0.0f)
    : x(x)
    , y(y)
    , z(z)
    , d(d)
  {

  }

  VK_FORCEINLINE vkPlane(const vkVector3f &pos, const vkVector3f &norm)
  {
    vkVector3f nn;
    norm.Normalized(nn);
    x = nn.x;
    y = nn.y;
    z = nn.z;
    d = -pos.Dot(nn);
  }

  VK_FORCEINLINE float Distance(const vkVector3f &pos) const
  {
    return x * pos.x + y * pos.y + z * pos.z + d;
  }


  VK_FORCEINLINE vkVector3f& ReflectPoint(const vkVector3f &p, vkVector3f &r) const
  {
    vkVector3f origin, diff;
    vkVector3f normal = vkVector3f(x, y, z);
    vkVector3f::Mul(normal, -d, origin);
    vkVector3f::Sub(p, origin, diff);

    vkVector3f::Mul(normal, 2.0f * diff.Dot(normal), r);
    vkVector3f::Sub(diff, r, r);
    return vkVector3f::Add(r, origin, r);
  }

  vkVector3f &ReflectDirection(const vkVector3f &d, vkVector3f &r) const
  {
    vkVector3f normal = vkVector3f(x, y, z);
    vkVector3f::Mul(normal, 2.0f * d.Dot(normal), r);
    return vkVector3f::Sub(d, r, r);
  }


}; 