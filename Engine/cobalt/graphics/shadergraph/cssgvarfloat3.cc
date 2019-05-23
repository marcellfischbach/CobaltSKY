#include <cobalt/graphics/shadergraph/cssgvarfloat3.hh>

cs::SGVarFloat3::SGVarFloat3()
  : cs::SGResourceNode()
{
  SetName(CS_VAR_FLOAT3_NAME);
  SetResourceType(cs::eSPT_Vector3);
  AddOutput(new cs::SGOutput(cs::eSGDT_Float3, "v"));
}
