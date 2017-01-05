#include <Valkyrie/graphics/shadergraph/vksgvarfloat4.hh>

vkSGVarFloat4::vkSGVarFloat4()
  : vkSGResourceNode()
{
  SetName(VK_VAR_FLOAT4_NAME);
  SetResourceType(eSPT_Vector4);
  AddOutput(new vkSGOutput(eSGDT_Float4, "v"));
}
