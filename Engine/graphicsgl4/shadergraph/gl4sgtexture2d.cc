#include <graphicsgl4/shadergraph/gl4sgtexture2d.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <sstream>

void cs::SGTexture2DGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *txt = GetNode();
  cs::SGResourceNode *res = cs::QueryClass<cs::SGResourceNode>(txt);
  if (!res)
  {
    return;
  }
  std::string bindingName = res->GetResourceName();
  if (bindingName.length() == 0)
  {
    return;
  }

  std::string txtCoordinate = ctx.GetDefaultTextureCoordinate();
  if (ctx.HasInputValue(txt->GetInput("uv")))
  {
    txtCoordinate = ctx.GetFullInputValue(txt->GetInput("uv"));
  }

  bindingName = "cs_" + bindingName;
  std::ostringstream ss;
  ss << "texture(" << bindingName << ", " << txtCoordinate << ")";
  std::string code = ss.str();

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


