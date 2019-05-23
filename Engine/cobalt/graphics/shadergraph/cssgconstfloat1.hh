#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat1.refl.hh>


#define CS_CONST_FLOAT_NAME "Const/Float"

namespace cs
{

CS_CLASS()
class CSE_API SGConstFloat : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGConstFloat();
  virtual ~SGConstFloat() { };
};

}
