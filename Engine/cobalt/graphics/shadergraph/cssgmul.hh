#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgmul.refl.hh>

#define CS_MUL_NAME "Math/Mul"

namespace cs
{

CS_CLASS()
class CSE_API SGMul : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGMul();
  virtual ~SGMul() { }

  virtual bool Validate();

};

}

