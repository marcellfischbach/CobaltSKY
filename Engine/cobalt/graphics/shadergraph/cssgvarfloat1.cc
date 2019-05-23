#include <cobalt/graphics/shadergraph/cssgvarfloat1.hh>

cs::SGVarFloat::SGVarFloat()
  : cs::SGResourceNode()
{
  SetName(CS_VAR_FLOAT_NAME);
  SetResourceType(cs::eSPT_Float);
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "v"));
}
