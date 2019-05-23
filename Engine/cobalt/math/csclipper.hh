#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csvector3f.hh>
#include <vector>

namespace cs
{

class CSE_API Clipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~Clipper();

  virtual ClipResult Test(const cs::Vector3f& pos) const = 0;
  virtual ClipResult Test(const cs::BoundingBox& bbox, bool debug = false) const = 0;


protected:
  Clipper();


};


}