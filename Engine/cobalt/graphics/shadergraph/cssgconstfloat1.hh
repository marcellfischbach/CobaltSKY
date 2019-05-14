#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat1.refl.hh>


#define CS_CONST_FLOAT_NAME "Const/Float"


CS_CLASS()
class CSE_API csSGConstFloat : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGConstFloat();
  virtual ~csSGConstFloat() { };
};