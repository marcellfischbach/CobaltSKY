
#include <cobalt/graphics/shadergraph/cssgfloat3.hh>

csSGFloat3::csSGFloat3()
  : csSGNode()
{
  SetName(CS_FLOAT3_NAME);
  AddInput(new csSGInput("x", true, true));
  AddInput(new csSGInput("y", true, true));
  AddInput(new csSGInput("z", true, true));
  AddOutput(new csSGOutput(eSGDT_Float3, "v", ""));
}
