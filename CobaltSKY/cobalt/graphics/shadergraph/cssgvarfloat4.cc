#include <cobalt/graphics/shadergraph/cssgvarfloat4.hh>

csSGVarFloat4::csSGVarFloat4()
  : csSGResourceNode()
{
  SetName(CS_VAR_FLOAT4_NAME);
  SetResourceType(eSPT_Vector4);
  AddOutput(new csSGOutput(eSGDT_Float4, "v"));
}
