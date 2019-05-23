
#include <cobalt/graphics/shadergraph/cssgfloat3.hh>

cs::SGFloat3::SGFloat3()
  : cs::SGNode()
{
  SetName(CS_FLOAT3_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddInput(new cs::SGInput("y", true, true));
  AddInput(new cs::SGInput("z", true, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float3, "v", ""));
}
