#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsub.refl.hh>

#define CS_SUB_NAME "Math/Sub"


namespace cs
{

CS_CLASS()
class CSE_API SGSub : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGSub();
  virtual ~SGSub() { }

  virtual bool Validate();

};

}
