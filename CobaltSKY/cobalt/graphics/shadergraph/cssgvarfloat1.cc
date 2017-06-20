#include <cobalt/graphics/shadergraph/cssgvarfloat1.hh>

csSGVarFloat::csSGVarFloat()
  : csSGResourceNode()
{
  SetName(CS_VAR_FLOAT_NAME);
  SetResourceType(eSPT_Float);
  AddOutput(new csSGOutput(eSGDT_Float, "v"));
}
