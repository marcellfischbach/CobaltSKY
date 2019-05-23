
#include <cobalt/graphics/shadergraph/cssgconstfloat3.hh>


cs::SGConstFloat3::SGConstFloat3()
  : cs::SGNode()
{
  SetName(CS_CONST_FLOAT3_NAME);
  AddInput(new cs::SGInput("x", true, false));
  AddInput(new cs::SGInput("y", true, false));
  AddInput(new cs::SGInput("z", true, false));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float3, "v", ""));
}
