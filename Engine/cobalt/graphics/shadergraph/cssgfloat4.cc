
#include <cobalt/graphics/shadergraph/cssgfloat4.hh>

cs::SGFloat4::SGFloat4()
  : cs::SGNode()
{
  SetName(CS_FLOAT4_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddInput(new cs::SGInput("y", true, true));
  AddInput(new cs::SGInput("z", true, true));
  AddInput(new cs::SGInput("w", true, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float4, "v", ""));
}
