
#include <cobalt/graphics/shadergraph/cssgconstfloat4.hh>


csSGConstFloat4::csSGConstFloat4()
  : csSGNode()
{
  SetName(CS_CONST_FLOAT4_NAME);
  AddInput(new csSGInput("x", true, false));
  AddInput(new csSGInput("y", true, false));
  AddInput(new csSGInput("z", true, false));
  AddInput(new csSGInput("w", true, false));
  AddOutput(new csSGOutput(eSGDT_Float4, "v", ""));
}
