#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat4.refl.hh>

#define CS_FLOAT4_NAME "Assemble/Float4"

namespace cs
{

CS_CLASS()
class CSE_API SGFloat4 : public CS_SUPER(cs::SGNode)
{
  CS_CLASS_GEN;
public:
  SGFloat4();
  virtual ~SGFloat4() { };
};

}
