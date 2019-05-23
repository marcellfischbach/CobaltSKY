
#include <cobalt/graphics/shadergraph/cssgoneminus.hh>




cs::SGOneMinus::SGOneMinus()
  : cs::SGNode()
{
  SetName(CS_ONE_MINUS_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddOutput(new cs::SGOutput("v"));
}


bool cs::SGOneMinus::Validate()
{
  cs::eSGDataType dtA = GetInputDataType(0);
  if (dtA == cs::eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
