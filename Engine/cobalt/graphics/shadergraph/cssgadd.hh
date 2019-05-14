
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgadd.refl.hh>

#define CS_ADD_NAME "Math/Add"

CS_CLASS()
class CSE_API csSGAdd : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGAdd();
  virtual ~csSGAdd() { }

  virtual bool Validate();

};
