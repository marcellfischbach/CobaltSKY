#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat3.refl.hh>


#define CS_VAR_FLOAT3_NAME "Var/Float3"

CS_CLASS()
class CSE_API csSGVarFloat3 : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:

  csSGVarFloat3();
  virtual ~csSGVarFloat3() { }
};