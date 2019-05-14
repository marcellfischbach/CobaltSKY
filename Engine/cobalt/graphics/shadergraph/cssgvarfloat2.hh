#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat2.refl.hh>


#define CS_VAR_FLOAT2_NAME "Var/Float2"

CS_CLASS()
class CSE_API csSGVarFloat2 : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:

  csSGVarFloat2();
  virtual ~csSGVarFloat2() { }
};