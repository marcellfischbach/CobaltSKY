
#include <cobalt/graphics/shadergraph/cssgconstfloat3.hh>


csSGConstFloat3::csSGConstFloat3()
  : csSGNode()
{
  SetName(CS_CONST_FLOAT3_NAME);
  AddInput(new csSGInput("x", true, false));
  AddInput(new csSGInput("y", true, false));
  AddInput(new csSGInput("z", true, false));
  AddOutput(new csSGOutput(eSGDT_Float3, "v", ""));
}
