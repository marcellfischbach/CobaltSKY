#include <GraphicsGL4/ShaderGraph/gl4sgtexture2d.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>
#include <sstream>

void vkSGTexture2DGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *txt = GetNode();
  vkSGResourceNode *res = vkQueryClass<vkSGResourceNode>(txt);
  if (!res)
  {
    return;
  }
  vkString bindingName = res->GetResourceName();
  if (bindingName.length() == 0)
  {
    return;
  }

  vkString txtCoordinate = ctx.GetDefaultTextureCoordinate();
  if (ctx.HasInputValue(txt->GetInput("uv")))
  {
    txtCoordinate = ctx.GetFullInputValue(txt->GetInput("uv"));
  }

  bindingName = "vk_" + bindingName;
  std::ostringstream ss;
  ss << "texture(" << bindingName << ", " << txtCoordinate << ")";
  vkString code = ss.str();

  if (!IsInline())
  {
    code = ctx.AddAssignment("vec4", code);
  }

  ctx.SetOutputValue(txt->GetOutput("c"), code);
  ctx.SetOutputValue(txt->GetOutput("r"), code);
  ctx.SetOutputValue(txt->GetOutput("g"), code);
  ctx.SetOutputValue(txt->GetOutput("b"), code);
  ctx.SetOutputValue(txt->GetOutput("a"), code);

  ctx.AddBinding(txt, "sampler2D", bindingName);
}


