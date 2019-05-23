#include <cobalt/graphics/shadergraph/cssgtexture2d.hh>



cs::SGTexture2D::SGTexture2D()
  : cs::SGResourceNode()
{
  SetName(CS_TEXTURE2D_NAME);
  SetResourceType(cs::eSPT_Texture);

  AddInput(new cs::SGInput("uv", false, true));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float4, "c"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "r", "r"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "g", "g"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "b", "b"));
  AddOutput(new cs::SGOutput(cs::eSGDT_Float, "a", "a"));

}
