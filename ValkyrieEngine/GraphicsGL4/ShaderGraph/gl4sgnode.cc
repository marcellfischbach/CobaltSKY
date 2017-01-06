
#include <GraphicsGL4/ShaderGraph/gl4sgnode.hh>
#include <GraphicsGL4/ShaderGraph/gl4sgshadergraphctx.hh>

vkSGNodeGL4::vkSGNodeGL4()
  : vkObject()
  , m_inline(true)
  , m_forceInline(false)
  , m_inlineEvaluated(false)
  , m_evaluated(false)
  , m_doubleInlineEvaluateInput(false)
{

}

vkSGNodeGL4::~vkSGNodeGL4()
{

}




vkString vkSGNodeGL4::AssignOutput(vkShaderGraphCtx &ctx, vkSGOutput *output, const vkString &exp, const vkString &type)
{
  if (IsInline())
  {
    ctx.SetOutputValue(output, exp);
    return exp;
  }
  else
  {
    vkString var = ctx.AddAssignment(type, exp);
    ctx.SetOutputValue(output, var);
    return var;
  }
}

vkSGDataType vkSGNodeGL4::GetHigher(vkSGDataType dtA, vkSGDataType dtB)
{
  return dtA > dtB ? dtA : dtB;
}

vkString vkSGNodeGL4::GetDataTypeVar(vkSGDataType dt)
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

void vkSGNodeGL4::SetDoubleInlineEvaluateInput()
{
  m_doubleInlineEvaluateInput = true;
}

bool vkSGNodeGL4::EvaluateInline(vkShaderGraphCtx &ctx)
{
  if (!m_inlineEvaluated)
  {
    m_inlineEvaluated = true;
    for (vkSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
    {
      vkSGInput *input = m_node->GetInput(i);
      vkSGNode *node = input->GetNode();

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

bool vkSGNodeGL4::Evaluate(vkShaderGraphCtx &ctx)
{
  if (m_evaluated)
  {
    return false;
  }
  m_evaluated = true;
  for (vkSize i = 0, in = m_node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = m_node->GetInput(i);
    ctx.Evaluate(input);
  }

  PrivEvaluate(ctx);

  return true;
}




vkString vkSGNodeGL4::GetFloat(vkShaderGraphCtx &ctx, int x)
{
  vkSGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0.0f);
  }

  if (input->CanInputNode())
  {
    vkSGOutput *output = input->GetInput();
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


vkString vkSGNodeGL4::GetInt(vkShaderGraphCtx &ctx, int x)
{
  vkSGInput *input = m_node->GetInput(x);
  if (!input)
  {
    return std::to_string(0);
  }

  if (input->CanInputNode())
  {
    vkSGOutput *output = input->GetInput();
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
