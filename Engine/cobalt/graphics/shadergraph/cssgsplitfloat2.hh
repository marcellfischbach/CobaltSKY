#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.refl.hh>

#define CS_SPLIT_FLOAT2_NAME "Assemble/SplitFloat2"

namespace cs
{

CS_CLASS()
class CSE_API SGSplitFloat2 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGSplitFloat2();
  virtual ~SGSplitFloat2() { }

  virtual bool Validate();
};

}