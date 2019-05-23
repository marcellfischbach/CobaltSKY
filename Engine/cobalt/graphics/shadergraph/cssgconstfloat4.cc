
#include <cobalt/graphics/shadergraph/cssgconstfloat4.hh>


cs::SGConstFloat4::SGConstFloat4()
  : cs::SGNode()
{
  SetName(CS_CONST_FLOAT4_NAME);
  AddInput(new cs::SGInput("x", true, false));
  AddInput(new cs::SGInput("y", true, false));
  AddInput(new cs::SGInput("z", true, false));
  AddInput(new cs::SGInput("w", true, false));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float4, "v", ""));
}
