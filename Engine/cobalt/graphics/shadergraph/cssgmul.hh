#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgmul.refl.hh>

#define CS_MUL_NAME "Math/Mul"

CS_CLASS()
class CSE_API csSGMul : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGMul();
  virtual ~csSGMul() { }

  virtual bool Validate();

};
