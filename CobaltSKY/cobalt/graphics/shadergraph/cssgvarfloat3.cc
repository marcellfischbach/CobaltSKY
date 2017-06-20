#include <cobalt/graphics/shadergraph/cssgvarfloat3.hh>

csSGVarFloat3::csSGVarFloat3()
  : csSGResourceNode()
{
  SetName(CS_VAR_FLOAT3_NAME);
  SetResourceType(eSPT_Vector3);
  AddOutput(new csSGOutput(eSGDT_Float3, "v"));
}
