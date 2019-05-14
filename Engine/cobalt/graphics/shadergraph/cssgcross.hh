
#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgcross.refl.hh>

#define CS_CROSS_NAME "Math/Cross"

CS_CLASS()
class CSE_API csSGCross : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGCross();
  virtual ~csSGCross() { }

  virtual bool Validate();

};
