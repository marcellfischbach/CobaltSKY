
#include <cobalt/graphics/shadergraph/cssgfloat2.hh>

cs::SGFloat2::SGFloat2()
  : cs::SGNode()
{
  SetName(CS_FLOAT2_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddInput(new cs::SGInput("y", true, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float2, "v", ""));
}
