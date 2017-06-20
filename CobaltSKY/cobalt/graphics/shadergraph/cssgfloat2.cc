
#include <cobalt/graphics/shadergraph/cssgfloat2.hh>

csSGFloat2::csSGFloat2()
  : csSGNode()
{
  SetName(CS_FLOAT2_NAME);
  AddInput(new csSGInput("x", true, true));
  AddInput(new csSGInput("y", true, true));
  AddOutput(new csSGOutput(eSGDT_Float2, "v", ""));
}
