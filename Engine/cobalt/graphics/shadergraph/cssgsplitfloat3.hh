#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.refl.hh>

#define CS_SPLIT_FLOAT3_NAME "Assemble/SplitFloat3"

CS_CLASS()
class CSE_API csSGSplitFloat3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGSplitFloat3();
  virtual ~csSGSplitFloat3() { }

  virtual bool Validate();
};
