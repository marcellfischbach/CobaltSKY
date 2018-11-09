
#include <graphicsgl4/shadergraph/gl4sgvar.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <cobalt/graphics/shadergraph/cssgresourcenode.hh>


csSGVarGL4::csSGVarGL4(const std::string &typeName)
  : csSGNodeGL4()
  , m_typeName(typeName)
{

}

void csSGVarGL4::PrivEvaluate(csShaderGraphCtx &ctx)
{
  csSGNode *fl = GetNode();
  csSGResourceNode *res = csQueryClass<csSGResourceNode>(fl);
  if (res)
  {
    std::string bindingName = "cs_" + res->GetResourceName();
    ctx.SetOutputValue(fl->GetOutput("v"), bindingName);

    ctx.AddBinding(fl, m_typeName, bindingName);
  }
}
