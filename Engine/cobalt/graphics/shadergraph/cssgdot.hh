#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdot.refl.hh>

#define CS_DOT_NAME "Math/Dot"

namespace cs
{

CS_CLASS()
class CSE_API SGDot : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGDot();
  virtual ~SGDot() { }

  virtual bool Validate();

};

}
