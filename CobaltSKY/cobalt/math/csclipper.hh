#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csboundingbox.hh>
#include <cobalt/math/csvector3f.hh>
#include <vector>

class CSE_API csClipper
{
public:
  enum ClipResult
  {
    eCR_In,
    eCR_Out,
    eCR_Intermediate,
  };
public:
  virtual ~csClipper();

  virtual ClipResult Test(const csVector3f &pos) const = 0;
  virtual ClipResult Test(const csBoundingBox &bbox, bool debug = false) const = 0;


protected:
  csClipper();


};

