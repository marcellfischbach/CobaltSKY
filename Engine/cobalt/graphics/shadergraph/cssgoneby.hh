#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgoneby.refl.hh>

#define CS_ONE_BY_NAME "Math/1 by X"

CS_CLASS()
class CSE_API csSGOneBy : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGOneBy();
  virtual ~csSGOneBy() { }

  virtual bool Validate();

};
