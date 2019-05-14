#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdot.refl.hh>

#define CS_DOT_NAME "Math/Dot"

CS_CLASS()
class CSE_API csSGDot : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGDot();
  virtual ~csSGDot() { }

  virtual bool Validate();

};
