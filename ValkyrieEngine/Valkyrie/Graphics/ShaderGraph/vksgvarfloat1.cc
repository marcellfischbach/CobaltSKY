#include <Valkyrie/Graphics/ShaderGraph/vksgvarfloat1.hh>

vkSGVarFloat::vkSGVarFloat()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT_NAME);
  SetResourceType(eSPT_Float);
  AddOutput(new vkSGOutput(eSGDT_Float, "v"));
}
