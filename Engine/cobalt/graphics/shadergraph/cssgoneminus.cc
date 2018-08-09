
#include <cobalt/graphics/shadergraph/cssgoneminus.hh>




csSGOneMinus::csSGOneMinus()
  : csSGNode()
{
  SetName(CS_ONE_MINUS_NAME);
  AddInput(new csSGInput("x", true, true));
  AddOutput(new csSGOutput("v"));
}


bool csSGOneMinus::Validate()
{
  csSGDataType dtA = GetInputDataType(0);
  if (dtA == eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
