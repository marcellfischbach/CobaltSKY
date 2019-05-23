#include <cobalt/graphics/shadergraph/cssgvarfloat4.hh>


cs::SGVarFloat4::SGVarFloat4()
  : cs::SGResourceNode()
{
  SetName(CS_VAR_FLOAT4_NAME);
  SetResourceType(cs::eSPT_Vector4);
  AddOutput(new cs::SGOutput(cs::eSGDT_Float4, "v"));
}
