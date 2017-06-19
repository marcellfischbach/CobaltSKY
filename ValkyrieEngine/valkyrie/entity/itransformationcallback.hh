#pragma once

#include <valkyrie/csexport.hh>
#include <valkyrie/math/csmatrix.hh>

struct CSE_API iTransformationCallback
{
  virtual ~iTransformationCallback() { }

  virtual void TransformationChanged(const csMatrix4f &transformation) = 0;

};
