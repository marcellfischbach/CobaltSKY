#include <cobalt/graphics/shadergraph/cssgtexture2d.hh>



csSGTexture2D::csSGTexture2D()
  : csSGResourceNode()
{
  SetName(CS_TEXTURE2D_NAME);
  SetResourceType(eSPT_Texture);

  AddInput(new csSGInput("uv", false, true));
  AddOutput(new csSGOutput(eSGDT_Float4, "c"));
  AddOutput(new csSGOutput(eSGDT_Float, "r", "r"));
  AddOutput(new csSGOutput(eSGDT_Float, "g", "g"));
  AddOutput(new csSGOutput(eSGDT_Float, "b", "b"));
  AddOutput(new csSGOutput(eSGDT_Float, "a", "a"));

}
