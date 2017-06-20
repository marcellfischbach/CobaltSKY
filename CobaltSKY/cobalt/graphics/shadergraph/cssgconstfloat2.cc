
#include <cobalt/graphics/shadergraph/cssgconstfloat2.hh>


csSGConstFloat2::csSGConstFloat2()
  : csSGNode()
{
  SetName(CS_CONST_FLOAT2_NAME);
  AddInput(new csSGInput("x", true, false));
  AddInput(new csSGInput("y", true, false));
  AddOutput(new csSGOutput(eSGDT_Float2, "v", ""));
}
