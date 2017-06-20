#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <cobalt/graphics/shadergraph/cssgvarfloat4.refl.hh>


#define CS_VAR_FLOAT4_NAME "Var/Float4"

CS_CLASS()
class CSE_API csSGVarFloat4 : public CS_SUPER(csSGResourceNode)
{
  CS_CLASS_GEN;
public:

  csSGVarFloat4();
  virtual ~csSGVarFloat4() { }
};