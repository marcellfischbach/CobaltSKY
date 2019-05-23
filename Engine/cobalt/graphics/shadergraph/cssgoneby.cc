
#include <cobalt/graphics/shadergraph/cssgoneby.hh>




cs::SGOneBy::SGOneBy()
  : cs::SGNode()
{
  SetName(CS_ONE_BY_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddOutput(new cs::SGOutput("v"));
}


bool cs::SGOneBy::Validate()
{
  cs::eSGDataType dtA = GetInputDataType(0);
  if (dtA == cs::eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
