#include <cobalt/graphics/shadergraph/cssgvarfloat2.hh>

csSGVarFloat2::csSGVarFloat2()
  : csSGResourceNode()
{
  SetName(CS_VAR_FLOAT2_NAME);
  SetResourceType(eSPT_Vector2);
  AddOutput(new csSGOutput(eSGDT_Float2, "v"));
}
