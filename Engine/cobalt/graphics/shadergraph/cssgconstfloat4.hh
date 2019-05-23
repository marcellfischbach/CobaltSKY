#pragma once

#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgconstfloat4.refl.hh>



#define CS_CONST_FLOAT4_NAME "Const/Float4"

namespace cs
{

CS_CLASS()
class CSE_API SGConstFloat4 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGConstFloat4();
  virtual ~SGConstFloat4() { };
};

}
