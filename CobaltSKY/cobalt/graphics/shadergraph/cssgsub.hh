#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgsub.refl.hh>

#define CS_SUB_NAME "Math/Sub"

CS_CLASS()
class CSE_API csSGSub : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGSub();
  virtual ~csSGSub() { }

  virtual bool Validate();

};
