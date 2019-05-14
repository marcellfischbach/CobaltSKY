#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgdiv.refl.hh>

#define CS_DIV_NAME "Math/Div"

CS_CLASS()
class CSE_API csSGDiv : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGDiv();
  virtual ~csSGDiv() { }

  virtual bool Validate();

};
