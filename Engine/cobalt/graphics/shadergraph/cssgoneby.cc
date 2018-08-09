
#include <cobalt/graphics/shadergraph/cssgoneby.hh>




csSGOneBy::csSGOneBy()
  : csSGNode()
{
  SetName(CS_ONE_BY_NAME);
  AddInput(new csSGInput("x", true, true));
  AddOutput(new csSGOutput("v"));
}


bool csSGOneBy::Validate()
{
  csSGDataType dtA = GetInputDataType(0);
  if (dtA == eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
