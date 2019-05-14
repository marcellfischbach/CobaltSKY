#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat2.refl.hh>

#define CS_FLOAT2_NAME "Assemble/Float2"

CS_CLASS()
class CSE_API csSGFloat2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGFloat2();
  virtual ~csSGFloat2() { };
};
