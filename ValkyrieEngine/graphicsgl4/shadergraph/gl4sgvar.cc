
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <valkyrie/graphics/shadergraph/vksgresourcenode.hh>


vkSGVarGL4::vkSGVarGL4(const vkString &typeName)
  : vkSGNodeGL4()
  , m_typeName(typeName)
{

}

void vkSGVarGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *fl = GetNode();
  vkSGResourceNode *res = vkQueryClass<vkSGResourceNode>(fl);
  if (res)
  {
    vkString bindingName = "vk_" + res->GetResourceName();
    ctx.SetOutputValue(fl->GetOutput("v"), bindingName);

    ctx.AddBinding(fl, m_typeName, bindingName);
  }
}
