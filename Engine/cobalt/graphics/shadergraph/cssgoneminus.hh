#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgoneminus.refl.hh>

#define CS_ONE_MINUS_NAME "Math/1 minus X"

CS_CLASS()
class CSE_API csSGOneMinus : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGOneMinus();
  virtual ~csSGOneMinus() { }

  virtual bool Validate();

};
