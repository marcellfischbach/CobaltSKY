#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat2.refl.hh>

#define CS_FLOAT2_NAME "Assemble/Float2"

namespace cs
{


CS_CLASS()
class CSE_API SGFloat2 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGFloat2();
  virtual ~SGFloat2() { };
};

}