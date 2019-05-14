#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgneg.refl.hh>

#define CS_NEG_NAME "Math/Neg"

CS_CLASS()
class CSE_API csSGNeg : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGNeg();
  virtual ~csSGNeg() { }

  virtual bool Validate();

};
