#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat4.refl.hh>

#define CS_FLOAT4_NAME "Assemble/Float4"

CS_CLASS()
class CSE_API csSGFloat4 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGFloat4();
  virtual ~csSGFloat4() { };
};
