#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat1.refl.hh>


#define CS_VAR_FLOAT_NAME "Var/Float"

CS_CLASS()
class CSE_API csSGVarFloat : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:

  csSGVarFloat();
  virtual ~csSGVarFloat() { }
};