#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat3.refl.hh>



#define CS_CONST_FLOAT3_NAME "Const/Float3"

namespace cs
{

CS_CLASS()
class CSE_API SGConstFloat3 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGConstFloat3();
  virtual ~SGConstFloat3() { };
};

}
