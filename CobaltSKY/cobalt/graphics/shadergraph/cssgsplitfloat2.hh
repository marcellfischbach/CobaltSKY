#pragma once
#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.refl.hh>

#define CS_SPLIT_FLOAT2_NAME "Assemble/SplitFloat2"



CS_CLASS()
class CSE_API csSGSplitFloat2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGSplitFloat2();
  virtual ~csSGSplitFloat2() { }

  virtual bool Validate();
};
