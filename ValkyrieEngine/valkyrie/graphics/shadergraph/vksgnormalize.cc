
#include <valkyrie/graphics/shadergraph/vksgnormalize.hh>




vkSGNormalize::vkSGNormalize()
  : vkSGNode()
{
  SetName(VK_NORMALIZE_NAME);
  AddInput(new vkSGInput("a", true, true));
  AddOutput(new vkSGOutput("v"));
}


bool vkSGNormalize::Validate()
{
  bool success = vkSGNode::Validate();

  vkSGDataType dtA = GetInputDataType(0);

  if (!FloatType(dtA))
  {
    SetValidationMessage("Invalid input type");
    return false;
  }
  SetOutputDataType(0, dtA);
  return success;
}
