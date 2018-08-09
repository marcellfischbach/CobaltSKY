
#include <cobalt/graphics/shadergraph/cssgneg.hh>




csSGNeg::csSGNeg()
  : csSGNode()
{
  SetName(CS_NEG_NAME);
  AddInput(new csSGInput("x", true, true));
  AddOutput(new csSGOutput("v"));
}


bool csSGNeg::Validate()
{
  csSGDataType dtA = GetInputDataType(0);
  if (dtA == eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
