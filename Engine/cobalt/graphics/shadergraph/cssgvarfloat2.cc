#include <cobalt/graphics/shadergraph/cssgvarfloat2.hh>

cs::SGVarFloat2::SGVarFloat2()
  : cs::SGResourceNode()
{
  SetName(CS_VAR_FLOAT2_NAME);
  SetResourceType(cs::eSPT_Vector2);
  AddOutput(new cs::SGOutput(cs::eSGDT_Float2, "v"));
}
