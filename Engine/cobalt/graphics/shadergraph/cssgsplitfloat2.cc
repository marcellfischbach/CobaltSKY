
#include <cobalt/graphics/shadergraph/cssgsplitfloat2.hh>

cs::SGSplitFloat2::SGSplitFloat2()
  : cs::SGNode()
{
  SetName(CS_SPLIT_FLOAT2_NAME);
  AddInput(new cs::SGInput("v", false, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "x", "x"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "y", "y"));
}

bool cs::SGSplitFloat2::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dt = GetInputDataType(0);

  return success && dt == cs::eSGDT_Float2;
}

