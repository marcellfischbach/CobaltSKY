#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.refl.hh>


#define CS_SPLIT_FLOAT4_NAME "Assemble/SplitFloat4"

namespace cs
{

CS_CLASS()
class CSE_API SGSplitFloat4 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGSplitFloat4();
  virtual ~SGSplitFloat4() { }

  virtual bool Validate();
};

}
