
#include <cobalt/graphics/shadergraph/cssgneg.hh>




cs::SGNeg::SGNeg()
  : cs::SGNode()
{
  SetName(CS_NEG_NAME);
  AddInput(new cs::SGInput("x", true, true));
  AddOutput(new cs::SGOutput("v"));
}


bool cs::SGNeg::Validate()
{
  cs::eSGDataType dtA = GetInputDataType(0);
  if (dtA == cs::eSGDT_Inval)
  {
    return false;
  }
  SetOutputDataType(0, dtA);
  return true;
}
