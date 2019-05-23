#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat4.refl.hh>


#define CS_VAR_FLOAT4_NAME "Var/Float4"

namespace cs
{

CS_CLASS()
class CSE_API SGVarFloat4 : public CS_SUPER(cs::SGResourceNode)
{
  CS_CLASS_GEN;
public:

  SGVarFloat4();
  virtual ~SGVarFloat4() { }
};

}
