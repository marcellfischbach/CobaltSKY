#pragma once
#include <cobalt/csexport.hh>
#include <csrefl/class.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssgfloat3.refl.hh>

#define CS_FLOAT3_NAME "Assemble/Float3"

CS_CLASS()
class CSE_API csSGFloat3 : public CS_SUPER(csSGNode)
{
  CS_CLASS_GEN;
public:
  csSGFloat3();
  virtual ~csSGFloat3() { };
};
