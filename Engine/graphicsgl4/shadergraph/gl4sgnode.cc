
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>

csSGNodeGL4::csSGNodeGL4()
  : cs::Object()
  , m_inline(true)
  , m_forceInline(false)
  , m_inlineEvaluated(false)
  , m_evaluated(false)
  , m_doubleInlineEvaluateInput(false)
{

}

csSGNodeGL4::~csSGNodeGL4()
{

}




std::string csSGNodeGL4::AssignOutput(csShaderGraphCtx &ctx, cs::SGOutput *output, const std::string &exp, const std::string &type)
{
  if (IsInline())
  {
    ctx.SetOutputValue(output, exp);
    return exp;
  }
  else
  {
    std::string var = ctx.AddAssignment(type, exp);
    ctx.SetOutputValue(output, var);
    return var;
  }
}

cs::eSGDataType csSGNodeGL4::GetHigher(cs::eSGDataType dtA, cs::eSGDataType dtB)
{
  return dtA > dtB ? dtA : dtB;
}

std::string csSGNodeGL4::GetDataTypeVar(cs::eSGDataType dt)
{
  switch (dt)
  {
  case cs::eSGDT_Float:
    return "float";
  case cs::eSGDT_Float2:
    return "vec2";
  case cs::eSGDT_Float3:
    return "vec3";
  case cs::eSGDT_Float4:
    return "vec4";
  case cs::eSGDT_Int:
    return "int";
  case cs::eSGDT_Int2:
    return "uvec2";
  case cs::eSGDT_Int3:
    return "uvec3";
  case cs::eSGDT_Int4:
    return "uvec4";
  case cs::eSGDT_Texture1D:
    return "sampler1D";
  case cs::eSGDT_Texture1DArray:
    return "sampler1DArray";

  case cs::eSGDT_Texture2D:
    return "sampler2D";
  case cs::eSGDT_Texture2DArray:
    return "sampler2DArray";

  case cs::eSGDT_Texture3D:
    return "sampler3D";

  case cs::eSGDT_TextureCube:
    return "samplerCube";
  case cs::eSGDT_TextureCubeArray:
    return "samplerCubeArray";
  }
  return "inval";
}

void csSGNodeGL4::SetDoubleInlineEvaluateInput()
{
  m_doubleInlineEvaluateInput = true;
}

bool csSGNodeGL4::EvaluateInline(csShaderGraphCtx &ctx)
{
  if (!m_inlineEvaluated)
  {
    m_inlineEvaluated = true;
    for (csSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
    {
      cs::SGInput *input = m_node->GetInput(i);
      cs::SGNode *node = input->GetNode();

      ctx.EvaluateInline(input);
      if (m_doubleInlineEvaluateInput)
      {
        ctx.EvaluateInline(input);
      }
    }
    return true;
  }
  else
  {
    SetInline(false);
    return false;
  }
}

bool csSGNodeGL4::Evaluate(csShaderGraphCtx &ctx)
{
  if (m_evaluated)
  {
    return false;
  }
  m_evaluated = true;
  for (csSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
  {
    cs::SGInput *input = m_node->GetInput(i);
    ctx.Evaluate(input);
  }

  PrivEvaluate(ctx);

  return true;
}




std::string csSGNodeGL4::GetFloat(csShaderGraphCtx &ctx, int x)
{
  cs::SGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0.0f);
  }

  if (input->CanInputNode())
  {
    cs::SGOutput *output = input->GetInput();
    if (output)
    {
      return ctx.GetFullOutputValue(output);
    }
  }
  if (input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return std::to_string(0.0f);
}


std::string csSGNodeGL4::GetInt(csShaderGraphCtx &ctx, int x)
{
  cs::SGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0);
  }

  if (input->CanInputNode())
  {
    cs::SGOutput *output = input->GetInput();
    if (output)
    {
      return ctx.GetFullOutputValue(output);
    }
  }
  if (input->CanInputConst())
  {
    return std::to_string((int)input->GetConst());
  }
  return std::to_string(0);
}
