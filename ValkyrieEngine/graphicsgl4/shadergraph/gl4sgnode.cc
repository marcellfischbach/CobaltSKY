
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>

csSGNodeGL4::csSGNodeGL4()
  : csObject()
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




csString csSGNodeGL4::AssignOutput(csShaderGraphCtx &ctx, csSGOutput *output, const csString &exp, const csString &type)
{
  if (IsInline())
  {
    ctx.SetOutputValue(output, exp);
    return exp;
  }
  else
  {
    csString var = ctx.AddAssignment(type, exp);
    ctx.SetOutputValue(output, var);
    return var;
  }
}

csSGDataType csSGNodeGL4::GetHigher(csSGDataType dtA, csSGDataType dtB)
{
  return dtA > dtB ? dtA : dtB;
}

csString csSGNodeGL4::GetDataTypeVar(csSGDataType dt)
{
  switch (dt)
  {
  case eSGDT_Float:
    return "float";
  case eSGDT_Float2:
    return "vec2";
  case eSGDT_Float3:
    return "vec3";
  case eSGDT_Float4:
    return "vec4";
  case eSGDT_Int:
    return "int";
  case eSGDT_Int2:
    return "uvec2";
  case eSGDT_Int3:
    return "uvec3";
  case eSGDT_Int4:
    return "uvec4";
  case eSGDT_Texture1D:
    return "sampler1D";
  case eSGDT_Texture1DArray:
    return "sampler1DArray";

  case eSGDT_Texture2D:
    return "sampler2D";
  case eSGDT_Texture2DArray:
    return "sampler2DArray";

  case eSGDT_Texture3D:
    return "sampler3D";

  case eSGDT_TextureCube:
    return "samplerCube";
  case eSGDT_TextureCubeArray:
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
      csSGInput *input = m_node->GetInput(i);
      csSGNode *node = input->GetNode();

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
    csSGInput *input = m_node->GetInput(i);
    ctx.Evaluate(input);
  }

  PrivEvaluate(ctx);

  return true;
}




csString csSGNodeGL4::GetFloat(csShaderGraphCtx &ctx, int x)
{
  csSGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0.0f);
  }

  if (input->CanInputNode())
  {
    csSGOutput *output = input->GetInput();
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


csString csSGNodeGL4::GetInt(csShaderGraphCtx &ctx, int x)
{
  csSGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0);
  }

  if (input->CanInputNode())
  {
    csSGOutput *output = input->GetInput();
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
