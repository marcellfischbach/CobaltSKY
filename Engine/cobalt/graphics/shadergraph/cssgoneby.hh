#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgoneby.refl.hh>

#define CS_ONE_BY_NAME "Math/1 by X"

namespace cs
{

CS_CLASS()
class CSE_API SGOneBy : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGOneBy();
  virtual ~SGOneBy() { }

  virtual bool Validate();

};

}
