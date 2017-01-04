#include <Valkyrie/Graphics/ShaderGraph/vksgtexture2d.hh>



vkSGTexture2D::vkSGTexture2D()
  : vkSGResourceNode()
{
  SetName(VK_TEXTURE2D_NAME);
  SetResourceType(eSPT_Texture);

  AddInput(new vkSGInput("uv", false, true));
  AddOutput(new vkSGOutput(eSGDT_Float4, "c"));
  AddOutput(new vkSGOutput(eSGDT_Float, "r", "r"));
  AddOutput(new vkSGOutput(eSGDT_Float, "g", "g"));
  AddOutput(new vkSGOutput(eSGDT_Float, "b", "b"));
  AddOutput(new vkSGOutput(eSGDT_Float, "a", "a"));
}
