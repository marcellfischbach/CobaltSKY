#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat3.refl.hh>

#define CS_FLOAT3_NAME "Assemble/Float3"

namespace cs
{

CS_CLASS()
class CSE_API SGFloat3 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGFloat3();
  virtual ~SGFloat3() { };
};

}
