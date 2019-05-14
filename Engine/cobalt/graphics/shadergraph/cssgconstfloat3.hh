#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat3.refl.hh>



#define CS_CONST_FLOAT3_NAME "Const/Float3"


CS_CLASS()
class CSE_API csSGConstFloat3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGConstFloat3();
  virtual ~csSGConstFloat3() { };
}; 
