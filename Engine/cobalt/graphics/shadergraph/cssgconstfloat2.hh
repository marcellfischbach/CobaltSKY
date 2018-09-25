#pragma once

#include <cobalt/csexport.hh>
#include <cobalt/core/csclass.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat2.refl.hh>



#define CS_CONST_FLOAT2_NAME "Const/Float2"


CS_CLASS()
class CSE_API csSGConstFloat2 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGConstFloat2();
  virtual ~csSGConstFloat2() { };
};