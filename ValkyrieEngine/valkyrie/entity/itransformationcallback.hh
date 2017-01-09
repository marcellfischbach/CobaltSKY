#pragma once

#include <valkyrie/vkexport.hh>
#include <valkyrie/math/vkmatrix.hh>

struct VKE_API iTransformationCallback
{
  virtual ~iTransformationCallback() { }

  virtual void TransformationChanged(const vkMatrix4f &transformation) = 0;

};
