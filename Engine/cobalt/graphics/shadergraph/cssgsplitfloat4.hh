#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.refl.hh>


#define CS_SPLIT_FLOAT4_NAME "Assemble/SplitFloat4"


CS_CLASS()
class CSE_API csSGSplitFloat4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGSplitFloat4();
  virtual ~csSGSplitFloat4() { }

  virtual bool Validate();
};
