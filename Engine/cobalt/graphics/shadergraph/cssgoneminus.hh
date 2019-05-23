#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgoneminus.refl.hh>

#define CS_ONE_MINUS_NAME "Math/1 minus X"

namespace cs
{

CS_CLASS()
class CSE_API SGOneMinus : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGOneMinus();
  virtual ~SGOneMinus() { }

  virtual bool Validate();

};

}
