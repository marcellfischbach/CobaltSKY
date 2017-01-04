
#include <Valkyrie/Graphics/ShaderGraph/vksgconstfloat1.hh>


vkSGConstFloat::vkSGConstFloat()
  : vkSGNode()
{
  SetName(VK_CONST_FLOAT_NAME);
  AddInput(new vkSGInput("x", true, false));
  AddOutput(new vkSGOutput(eSGDT_Float, "v", ""));
}
