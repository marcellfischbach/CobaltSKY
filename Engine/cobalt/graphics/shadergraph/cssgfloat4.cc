
#include <cobalt/graphics/shadergraph/cssgfloat4.hh>

csSGFloat4::csSGFloat4()
  : csSGNode()
{
  SetName(CS_FLOAT4_NAME);
  AddInput(new csSGInput("x", true, true));
  AddInput(new csSGInput("y", true, true));
  AddInput(new csSGInput("z", true, true));
  AddInput(new csSGInput("w", true, true));
  AddOutput(new csSGOutput(eSGDT_Float4, "v", ""));
}
