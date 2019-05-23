
#include <cobalt/graphics/shadergraph/cssgsplitfloat3.hh>

cs::SGSplitFloat3::SGSplitFloat3()
  : cs::SGNode()
{
  SetName(CS_SPLIT_FLOAT3_NAME);
  AddInput(new cs::SGInput("v", false, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "x", "x"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "y", "y"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "z", "z"));
}

bool cs::SGSplitFloat3::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dt = GetInputDataType(0);

  return success && dt == cs::eSGDT_Float3;
}

