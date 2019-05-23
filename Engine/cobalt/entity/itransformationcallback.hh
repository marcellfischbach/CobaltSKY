#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/math/csmatrix.hh>

namespace cs
{

struct CSE_API iTransformationCallback
{
  virtual ~iTransformationCallback() { }

  virtual void TransformationChanged(const cs::Matrix4f& transformation) = 0;

};

}
