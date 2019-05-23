
#include <cobalt/graphics/shadergraph/cssgconstfloat1.hh>


cs::SGConstFloat::SGConstFloat()
  : cs::SGNode()
{
  SetName(CS_CONST_FLOAT_NAME);
  AddInput(new cs::SGInput("x", true, false));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "v", ""));
}
