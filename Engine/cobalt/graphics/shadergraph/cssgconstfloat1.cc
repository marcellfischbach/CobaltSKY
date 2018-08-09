
#include <cobalt/graphics/shadergraph/cssgconstfloat1.hh>


csSGConstFloat::csSGConstFloat()
  : csSGNode()
{
  SetName(CS_CONST_FLOAT_NAME);
  AddInput(new csSGInput("x", true, false));
  AddOutput(new csSGOutput(eSGDT_Float, "v", ""));
}
