#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.refl.hh>

#define CS_SPLIT_FLOAT3_NAME "Assemble/SplitFloat3"

namespace cs
{

CS_CLASS()
class CSE_API SGSplitFloat3 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGSplitFloat3();
  virtual ~SGSplitFloat3() { }

  virtual bool Validate();
};

}
