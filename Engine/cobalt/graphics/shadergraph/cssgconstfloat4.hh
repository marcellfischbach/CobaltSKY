#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat4.refl.hh>



#define CS_CONST_FLOAT4_NAME "Const/Float4"


CS_CLASS()
class CSE_API csSGConstFloat4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGConstFloat4();
  virtual ~csSGConstFloat4() { };
};