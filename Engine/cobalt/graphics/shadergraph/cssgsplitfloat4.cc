
#include <cobalt/graphics/shadergraph/cssgsplitfloat4.hh>

cs::SGSplitFloat4::SGSplitFloat4()
  : cs::SGNode()
{
  SetName(CS_SPLIT_FLOAT4_NAME);
  AddInput(new cs::SGInput("v", false, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "x", "x"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "y", "y"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "z", "z"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "w", "w"));
}

bool cs::SGSplitFloat4::Validate()
{
  bool success = cs::SGNode::Validate();

  cs::eSGDataType dt = GetInputDataType(0);

  return success && dt == cs::eSGDT_Float4;
}

