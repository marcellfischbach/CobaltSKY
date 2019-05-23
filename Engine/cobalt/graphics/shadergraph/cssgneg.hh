#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgneg.refl.hh>

#define CS_NEG_NAME "Math/Neg"

namespace cs
{

CS_CLASS()
class CSE_API SGNeg : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGNeg();
  virtual ~SGNeg() { }

  virtual bool Validate();

};

}
