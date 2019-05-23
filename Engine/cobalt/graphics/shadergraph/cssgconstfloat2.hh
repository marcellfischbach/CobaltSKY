#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat2.refl.hh>



#define CS_CONST_FLOAT2_NAME "Const/Float2"

namespace cs
{

CS_CLASS()
class CSE_API SGConstFloat2 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGConstFloat2();
  virtual ~SGConstFloat2() { };
};

}
