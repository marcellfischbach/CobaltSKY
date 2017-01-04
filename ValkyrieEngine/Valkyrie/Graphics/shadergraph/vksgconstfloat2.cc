
#include <Valkyrie/Graphics/shadergraph/vksgconstfloat2.hh>


vkSGConstFloat2::vkSGConstFloat2()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT2_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddInput(new vkSGInput("y", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float2, "v", ""));
}
