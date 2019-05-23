
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>


cs::SGVarGL4::SGVarGL4(const std::string &typeName)
  : cs::SGNodeGL4()
  , m_typeName(typeName)
{

}

void cs::SGVarGL4::PrivEvaluate(cs::ShaderGraphCtx &ctx)
{
  cs::SGNode *fl = GetNode();
  cs::SGResourceNode *res = cs::QueryClass<cs::SGResourceNode>(fl);
  if (res)
  {
    std::string bindingName = "cs_" + res->GetResourceName();
    ctx.SetOutputValue(fl->GetOutput("v"), bindingName);

    ctx.AddBinding(fl, m_typeName, bindingName);
  }
}
