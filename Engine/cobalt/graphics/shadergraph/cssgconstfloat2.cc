
#include <cobalt/graphics/shadergraph/cssgconstfloat2.hh>


cs::SGConstFloat2::SGConstFloat2()
  : cs::SGNode()
{
  SetName(CS_CONST_FLOAT2_NAME);
  AddInput(new cs::SGInput("x", true, false));
  AddInput(new cs::SGInput("y", true, false));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float2, "v", ""));
}
