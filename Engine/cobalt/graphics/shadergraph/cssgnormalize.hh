#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgnormalize.refl.hh>

#define CS_NORMALIZE_NAME "Math/Normalize"

namespace cs
{

CS_CLASS()
class CSE_API SGNormalize : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGNormalize();
  virtual ~SGNormalize() { }

  virtual bool Validate();

};

}
