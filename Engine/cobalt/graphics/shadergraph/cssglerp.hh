#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssglerp.refl.hh>

#define CS_LERP_NAME "Math/Lerp"

namespace cs
{

CS_CLASS()
class CSE_API SGLerp : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGLerp();
  virtual ~SGLerp() { }

  virtual bool Validate();

};

}
