#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat2.refl.hh>


#define CS_VAR_FLOAT2_NAME "Var/Float2"

namespace cs
{


CS_CLASS()
class CSE_API SGVarFloat2 : public CS_SUPER(cs::SGResourceNode)
{
  CS_CLASS_GEN;
public:

  SGVarFloat2();
  virtual ~SGVarFloat2() { }
};

}

