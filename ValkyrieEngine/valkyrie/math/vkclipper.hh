#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/math/vkboundingbox.hh>
#include <valkyrie/math/vkvector3f.hh>
#include <vector>

class VKE_API vkClipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~vkClipper();

  virtual ClipResult Test(const vkVector3f &pos) const = 0;
  virtual ClipResult Test(const vkBoundingBox &bbox, bool debug = false) const = 0;


protected:
  vkClipper();


};

