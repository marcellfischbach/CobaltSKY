
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgadd.refl.hh>

#define CS_ADD_NAME "Math/Add"

namespace cs
{

CS_CLASS()
class CSE_API SGAdd : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGAdd();
  virtual ~SGAdd() { }

  virtual bool Validate();

};

}