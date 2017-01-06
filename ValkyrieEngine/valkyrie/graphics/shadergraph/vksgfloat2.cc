
#include <valkyrie/graphics/shadergraph/vksgfloat2.hh>

vkSGFloat2::vkSGFloat2()
  : vkSGNode()
{
  SetName(VK_FLOAT2_NAME);
  AddInput(new vkSGInput("x", true, true));
  AddInput(new vkSGInput("y", true, true));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}
