#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat3.refl.hh>


#define CS_VAR_FLOAT3_NAME "Var/Float3"

namespace cs
{


CS_CLASS()
class CSE_API SGVarFloat3 : public CS_SUPER(cs::SGResourceNode)
{
  CS_CLASS_GEN;
public:

  SGVarFloat3();
  virtual ~SGVarFloat3() { }
};

}