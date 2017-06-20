#include <graphicsgl4/shadergraph/gl4sgtexture2d.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>
#include <sstream>

void csSGTexture2DGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *txt = GetNode();
  csSGResourceNode *res = csQueryClass<csSGResourceNode>(txt);
  if (!res)
  {
    return;
  }
  csString bindingName = res->GetResourceName();
  if (bindingName.length() == 0)
  {
    return;
  }

  csString txtCoordinate = ctx.GetDefaultTextureCoordinate();
  if (ctx.HasInputValue(txt->GetInput("uv")))
  {
    txtCoordinate = ctx.GetFullInputValue(txt->GetInput("uv"));
  }

  bindingName = "cs_" + bindingName;
  std::ostringstream ss;
  ss << "texture(" << bindingName << ", " << txtCoordinate << ")";
  csString code = ss.str();

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


