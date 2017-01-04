#pragma once

#include <Valkyrie/vkexport.hh>
#include <Valkyrie/Math/Matrix.hh>

struct VKE_API ITransformationCallback
{
  virtual ~ITransformationCallback() { }

  virtual void TransformationChanged(const vkMatrix4f &transformation) = 0;

};
