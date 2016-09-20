

#include <GraphicsGL4/ShaderGraph/ShaderGraphNodeGL4.hh>
#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <iostream>
#include <sstream>




vkShaderGraphCtx::vkShaderGraphCtx(vkShaderGraphGL4 *graph)
  : m_graph(graph)
  , m_code("")
  , m_variableCounter(0)
{

}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGNode *node)
{
  if (!node)
  {
    return 0;
  }

  vkSGNodeGL4 *glNode = m_nodes[node];
  if (!glNode)
  {
    const vkClass *cls = node->GetClass();
    glNode = m_graph->CreateNode(cls);
    if (glNode == 0)
    {
      return 0;
    }

    glNode->SetNode(node);
    m_nodes[node] = glNode;
  }
  return glNode;
}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return GetNode(output->GetNode());
}

vkSGNodeGL4 *vkShaderGraphCtx::GetNode(vkSGInput *input)
{
  if (!input)
  {
    return 0;
  }

  return GetNode(input->GetInput());
}

void vkShaderGraphCtx::Evaluate(vkSGNodeGL4 *node)
{
  if (node)
  {
    node->Evaluate(*this);
  }
}

void vkShaderGraphCtx::Evaluate(vkSGNode *node)
{
  Evaluate(GetNode(node));
}


void vkShaderGraphCtx::Evaluate(vkSGOutput *output)
{
  Evaluate(GetNode(output));
}

void vkShaderGraphCtx::Evaluate(vkSGInput *input)
{
  Evaluate(GetNode(input));
}


void vkShaderGraphCtx::EvaluateInline(vkSGNodeGL4 *node)
{
  if (node)
  {
    node->EvaluateInline(*this);
  }
}

void vkShaderGraphCtx::EvaluateInline(vkSGNode *node)
{
  EvaluateInline(GetNode(node));
}


void vkShaderGraphCtx::EvaluateInline(vkSGOutput *output)
{
  EvaluateInline(GetNode(output));
}

void vkShaderGraphCtx::EvaluateInline(vkSGInput *input)
{
  EvaluateInline(GetNode(input));
}



vkString vkShaderGraphCtx::CreateCode(vkSGOutput *output)
{
  vkSGNodeGL4 *node = GetNode(output->GetNode());
  EvaluateInline(node);
  Evaluate(node);

  vkString code = m_code;
  code += vkString("\n") + GetFullOutputValue(output).c_str() + vkString("\n");

  return code;
}

void vkShaderGraphCtx::EvaluateInlines(std::set<vkSGOutput*> outputs)
{
  for (vkSGOutput *output : outputs)
  {
    EvaluateInline(output);
  }
}

void vkShaderGraphCtx::GenerateCode(std::set<vkSGOutput*> outputs)
{
  m_code = "";
  for (vkSGOutput *output : outputs)
  {
    vkSGNode *node = output->GetNode();
    Evaluate(output);
  }
}

void vkShaderGraphCtx::AddExpression(const vkString &expression)
{
  m_code += expression + vkString(";\n");
}

vkString vkShaderGraphCtx::AddAssignment(const vkString &type, const vkString &statement)
{
  vkString var = GetNextVariable();
  vkString expression = type + vkString(" ") + var + vkString(" = ") + statement;
  AddExpression(expression);
  return var;
}


vkString vkShaderGraphCtx::GetNextVariable()
{
  static char buffer[64];
  m_variableCounter++;
  snprintf(buffer, 64, "v%03u", m_variableCounter);
  return vkString(buffer);
}


void vkShaderGraphCtx::AddBinding(vkSGNode *node, const vkString &variableType, const vkString &variableName)
{
  ExternalBinding binding;
  binding.variableName = variableName;
  binding.node = node;
  binding.variableType = variableType;
  m_unisformBindingNames[variableName] = binding;;
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGNode *node) const
{
  if (!node)
  {
    return false;
  }

  std::map<vkString, ExternalBinding>::const_iterator it = m_unisformBindingNames.find(bindingName);
  if (it != m_unisformBindingNames.end() && it->second.node == node)
  {
    return true;
  }

  for (vkSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    vkSGInput *input = node->GetInput(i);
    if (IsBindingApplyingFor(bindingName, input))
    {
      return true;
    }
  }
  return false;
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGOutput *output) const
{
  if (!output)
  {
    return false;
  }
  return IsBindingApplyingFor(bindingName, output->GetNode());
}

bool vkShaderGraphCtx::IsBindingApplyingFor(const vkString &bindingName, vkSGInput *input) const
{
  if (!input)
  {
    return false;
  }

  return IsBindingApplyingFor(bindingName, input->GetInput());
}

std::set<vkShaderGraphCtx::ExternalBinding> vkShaderGraphCtx::GetBindingsFor(std::set<vkSGOutput*> outputs) const
{
  std::set<ExternalBinding> result;
  std::map<vkString, ExternalBinding>::const_iterator it;
  for (it = m_unisformBindingNames.begin(); it != m_unisformBindingNames.end(); ++it)
  {
    for (vkSGOutput* output : outputs)
    {
      if (IsBindingApplyingFor(it->second.variableName, output))
      {
        result.insert(it->second);
      }
    }
  }

  return result;
}

void vkShaderGraphCtx::SetDefaultTextureCoordinate(const vkString &defaultTextureCoordinate)
{
  m_defaultTextureCoordinate = defaultTextureCoordinate;
}

const vkString &vkShaderGraphCtx::GetDefaultTextureCoordinate() const
{
  return m_defaultTextureCoordinate;
}

void vkShaderGraphCtx::SetOutputValue(vkSGOutput *output, const vkString &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
}

bool vkShaderGraphCtx::HasInputValue(vkSGInput *input) const
{
  return HasOutputValue(input ? input->GetInput() : 0);
}

bool vkShaderGraphCtx::HasOutputValue(vkSGOutput *output) const
{
  std::map<vkSGOutput *, vkString>::const_iterator it = m_outputValue.find(output);
  return it != m_outputValue.end();
}

vkString vkShaderGraphCtx::GetOutputValue(vkSGOutput *output)
{
  std::map<vkSGOutput *, vkString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return vkString("<inval>");
  }
  return it->second;
}

vkString vkShaderGraphCtx::GetInputValue(vkSGInput *input)
{
  return GetOutputValue(input ? input->GetInput() : 0);
}

vkString vkShaderGraphCtx::GetFullOutputValue(vkSGOutput *output)
{
  std::map<vkSGOutput *, vkString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return vkString("<inval>");
  }

  vkString res = it->second;

  if (output->GetAttr().length() != 0)
  {
    res = res += vkString(".") + output->GetAttr();
  }
  return res;
}

vkString vkShaderGraphCtx::GetFullInputValue(vkSGInput *input)
{
  if (input->GetInput() == 0 && input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return GetFullOutputValue(input ? input->GetInput() : 0);
}

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

void vkSGConstFloatGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  ctx.SetOutputValue(n->GetOutput(0), std::to_string(n->GetInput(0)->GetConst()));
}

void vkSGConstFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec2("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}

void vkSGConstFloat3GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec3("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst()) << ", "
    << std::to_string(n->GetInput(2)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}


void vkSGConstFloat4GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();

  std::ostringstream ss;
  ss << "vec4("
    << std::to_string(n->GetInput(0)->GetConst()) << ", "
    << std::to_string(n->GetInput(1)->GetConst()) << ", "
    << std::to_string(n->GetInput(2)->GetConst()) << ", "
    << std::to_string(n->GetInput(3)->GetConst())
    << ")";
  ctx.SetOutputValue(n->GetOutput(0), ss.str());
}

namespace
{
vkString eval(vkString *src, vkSGDataType *dataTypes, vkSGNode *node, vkShaderGraphCtx &ctx, int start, int num)
{
  if (start >= num)
  {
    return "";
  }

  int next = start;
  std::ostringstream ss;
  if (src[start].length() != 0)
  {

    int numEquals = 1;
    if (dataTypes[start] != eSGDT_Float && dataTypes[start] != eSGDT_Int)
    {
      for (int i = start + 1; i < num; ++i)
      {
        if (src[start] == src[i] && node->GetInput(i)->GetInput()->GetAttr().length() != 0)
        {
          numEquals++;
        }
        else
        {
          break;
        }
      }
    }
    if (numEquals == 1)
    {
      ss << ctx.GetFullInputValue(node->GetInput(start));
    }
    else
    {
      ss << src[start] << ".";
      for (int i = 0; i < numEquals; ++i)
      {
        ss << node->GetInput(start + i)->GetInput()->GetAttr();
      }
    }
    next += numEquals;
  }
  else
  {
    ss << std::to_string(node->GetInput(start)->GetConst());
    next++;
  }

  vkString trail = ::eval(src, dataTypes, node, ctx, next, num);
  if (trail.length() != 0)
  {
    ss << ", " << trail;
  }
  return ss.str();
}
}


void vkSGFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();
  vkString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : ""
  };

  vkSGDataType dt[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
  };
  std::ostringstream ss;
  if (src[0] == src[1] && src[0].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec2(" << eval(src, dt, n, ctx, 0, 2) << ")";
  }

  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec2");
}


void vkSGFloat3GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();
  vkString src[] = {
     n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
     n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : "",
     n->GetInput(2)->GetInput() ? ctx.GetInputValue(n->GetInput(2)) : ""
  };

  vkString attr[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetAttr() : "", 
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetAttr() : "",
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetAttr() : ""
  };

  vkSGDataType dt[] = {
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetDataType() : eSGDT_Inval,
  };



  std::ostringstream ss;
  if (src[0] == src[1] && src[0] == src[2] && src[0].length() != 0 && attr[0].length () != 0 && attr[1].length() != 0 && attr[2].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr()
      << n->GetInput(2)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec3(" << eval(src, dt, n, ctx, 0, 3) << ")";
  }

  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec3");
}


void vkSGFloat4GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *n = GetNode();
  vkString src[] = {
    n->GetInput(0)->GetInput() ? ctx.GetInputValue(n->GetInput(0)) : "",
    n->GetInput(1)->GetInput() ? ctx.GetInputValue(n->GetInput(1)) : "",
    n->GetInput(2)->GetInput() ? ctx.GetInputValue(n->GetInput(2)) : "",
    n->GetInput(3)->GetInput() ? ctx.GetInputValue(n->GetInput(3)) : ""
  };

  vkSGDataType dt[] = { 
    n->GetInput(0)->GetInput() ? n->GetInput(0)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(1)->GetInput() ? n->GetInput(1)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(2)->GetInput() ? n->GetInput(2)->GetInput()->GetDataType() : eSGDT_Inval,
    n->GetInput(3)->GetInput() ? n->GetInput(3)->GetInput()->GetDataType() : eSGDT_Inval,
  };
  std::ostringstream ss;
  if (src[0] == src[1] && src[0] == src[2] && src[0] == src[3] && src[0].length() != 0)
  {
    ss << src[0] << "."
      << n->GetInput(0)->GetInput()->GetAttr()
      << n->GetInput(1)->GetInput()->GetAttr()
      << n->GetInput(2)->GetInput()->GetAttr();
  }
  else
  {
    ss << "vec4(" << eval(src, dt, n, ctx, 0, 4) << ")";
  }


  AssignOutput(ctx, n->GetOutput(0), ss.str(), "vec4");
}

vkSGSplitFloat2GL4::vkSGSplitFloat2GL4()
  : vkSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void vkSGSplitFloat2GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *node = GetNode();
  vkSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  vkString exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);

}

vkSGSplitFloat3GL4::vkSGSplitFloat3GL4()
  : vkSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}


void vkSGSplitFloat3GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *node = GetNode();
  vkSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  vkString exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);
  ctx.SetOutputValue(node->GetOutput(2), exp);

}

vkSGSplitFloat4GL4::vkSGSplitFloat4GL4()
  : vkSGNodeGL4()
{
  SetDoubleInlineEvaluateInput();
}

void vkSGSplitFloat4GL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *node = GetNode();
  vkSGNodeGL4 *inputNode = ctx.GetNode(node->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  vkString exp = ctx.GetInputValue(node->GetInput(0));
  ctx.SetOutputValue(node->GetOutput(0), exp);
  ctx.SetOutputValue(node->GetOutput(1), exp);
  ctx.SetOutputValue(node->GetOutput(2), exp);
  ctx.SetOutputValue(node->GetOutput(3), exp);

}

void vkSGAddGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *add = GetNode();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = add->GetOutput(0)->GetDataType();// GetHigher(add->GetInput(0)->GetDataType(), add->GetInput(1)->GetDataType());
  AssignOutput(ctx, add->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}

void vkSGSubGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *sub = GetNode();
  ss << "(" << ctx.GetFullInputValue(sub->GetInput(0)).c_str() << " - " <<
    ctx.GetFullInputValue(sub->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = sub->GetOutput(0)->GetDataType();// GetHigher(sub->GetInput(0)->GetDataType(), sub->GetInput(1)->GetDataType());
  AssignOutput(ctx, sub->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}


void vkSGMulGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *mul = GetNode();
  ss << "(" << ctx.GetFullInputValue(mul->GetInput(0)).c_str() << " * " <<
    ctx.GetFullInputValue(mul->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = mul->GetOutput(0)->GetDataType();// GetHigher(mul->GetInput(0)->GetDataType(), mul->GetInput(1)->GetDataType());
  AssignOutput(ctx, mul->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}

void vkSGDivGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *div = GetNode();
  ss << "(" << ctx.GetFullInputValue(div->GetInput(0)).c_str() << " / " <<
    ctx.GetFullInputValue(div->GetInput(1)).c_str() <<
    ")";

  vkSGDataType dt = div->GetOutput(0)->GetDataType();// GetHigher(div->GetInput(0)->GetDataType(), div->GetInput(1)->GetDataType());
  AssignOutput(ctx, div->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}


void vkSGDotGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *dot = GetNode();
  ss << "dot(" << ctx.GetFullInputValue(dot->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(dot->GetInput(1)).c_str() <<
    ")";

  AssignOutput(ctx, dot->GetOutput(0), ss.str(), GetDataTypeVar(eSGDT_Float));
}

void vkSGCrossGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *dot = GetNode();
  ss << "cross(" << ctx.GetFullInputValue(dot->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(dot->GetInput(1)).c_str() <<
    ")";

  AssignOutput(ctx, dot->GetOutput(0), ss.str(), GetDataTypeVar(eSGDT_Float3));
}

void vkSGLerpGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *lerp = GetNode();
  ss << "mix(" << ctx.GetFullInputValue(lerp->GetInput(0)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(1)).c_str() << ", " <<
    ctx.GetFullInputValue(lerp->GetInput(2)).c_str() <<
    ")";
  vkSGDataType dt = lerp->GetOutput(0)->GetDataType();
  AssignOutput(ctx, lerp->GetOutput(0), ss.str(), GetDataTypeVar(dt));
}

void vkSGDefaultTextureCoordinateGL4::PrivEvaluate(vkShaderGraphCtx &ctx)
{
  vkSGNode *coord = GetNode();

  ctx.SetOutputValue(coord->GetOutput(0), ctx.GetDefaultTextureCoordinate());

}


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
    return ;
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


