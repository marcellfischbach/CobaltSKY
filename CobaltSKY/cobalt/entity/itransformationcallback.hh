#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix.hh>

struct CSE_API iTransformationCallback
{
  virtual ~iTransformationCallback() { }

  virtual void TransformationChanged(const csMatrix4f &transformation) = 0;

};
