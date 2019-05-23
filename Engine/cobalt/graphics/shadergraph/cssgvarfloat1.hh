#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat1.refl.hh>


#define CS_VAR_FLOAT_NAME "Var/Float"

namespace cs
{

CS_CLASS()
class CSE_API SGVarFloat : public CS_SUPER(cs::SGResourceNode)
{
  CS_CLASS_GEN;
public:

  SGVarFloat();
  virtual ~SGVarFloat() { }
};

}

