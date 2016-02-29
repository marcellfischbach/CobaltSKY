

#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <iostream>
#include <sstream>


vkShaderGraphGL4::vkShaderGraphGL4()
{

}


vkShaderGraphGL4::~vkShaderGraphGL4()
{

}

vkSGNodeGL4 *vkShaderGraphGL4::CreateNode(const vkClass *clazz) const
{
  std::map<const vkClass*, const vkClass*>::const_iterator it = m_nodeMapping.find(clazz);
  if (it == m_nodeMapping.end())
  {
    return new vkSGNodeGL4();
  }

  const vkClass *nodeClass = it->second;
  return nodeClass->CreateInstance<vkSGNodeGL4>();
}


vkString vkShaderGraphGL4::CreateCode(vkSGNode *node, vkSize output)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(output));
}

vkString vkShaderGraphGL4::CreateCode(vkSGNode *node, const vkString &outputName)
{
  if (!node)
  {
    return "<error>";
  }

  return CreateCode(node->GetOutput(outputName));
}


vkString vkShaderGraphGL4::CreateCode(vkSGOutput *output)
{
  vkShaderGraphCtx ctx(this);
  ctx.SetDefaultTextureCoordinate("inFragTexCoord");
  return ctx.CreateCode(output);
}



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
    glNode = m_graph->CreateNode(node->GetClass());
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


void vkShaderGraphCtx::AddBinding(const vkString &variableType, const vkString &variableName)
{
  m_unisformBindingNames[variableName] = variableType;
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
  return GetFullOutputValue(input ? input->GetInput() : 0);
}

vkSGNodeGL4::vkSGNodeGL4()
  : vkObject()
  , m_inline(true)
  , m_forceInline(false)
  , m_inlineEvaluated(false)
  , m_evaluated(false)
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

  // select the type of the node
  vkSGNodeType nodeClass = m_node->GetType();
  if (nodeClass == eSGNT_ConstFloat)
  {
    EvaluateConstFloat(ctx);
  }
  else if (nodeClass == eSGNT_ConstFloat3)
  {
    EvaluateConstFloat3(ctx);
  }
  else if (nodeClass == eSGNT_Float2)
  {
    EvaluateFloat2(ctx);
  }
  else if (nodeClass == eSGNT_Float3)
  {
    EvaluateFloat3(ctx);
  }
  else if (nodeClass == eSGNT_AddFloat3)
  {
    EvaluateAddFloat3(ctx);
  }
  else if (nodeClass == eSGNT_SplitFloat3)
  {
    EvaluateSplitFloat3(ctx);
  }
  else if (nodeClass == eSGNT_Texture2D)
  {
    EvaluateTexture2D(ctx);
  }


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
  if (input->CanConstData())
  {
    return std::to_string(input->GetFloat(0));
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
  if (input->CanConstData())
  {
    return std::to_string(input->GetInt(0));
  }
  return std::to_string(0);
}

void vkSGNodeGL4::EvaluateConstFloat(vkShaderGraphCtx &ctx)
{
  vkSGNode *fl = GetNode();
  ctx.SetOutputValue(fl->GetOutput(0), std::to_string(fl->GetInput(0)->GetFloat(0)));
}

void vkSGNodeGL4::EvaluateConstFloat3(vkShaderGraphCtx &ctx)
{
  vkSGNode *fl = GetNode();

  std::ostringstream ss;
  ss << "vec3(" <<
    GetFloat(ctx, 0) << ", " <<
    GetFloat(ctx, 1) << ", " <<
    GetFloat(ctx, 2) << 
    ")";

  ctx.SetOutputValue(fl->GetOutput(0), ss.str());

}

void vkSGNodeGL4::EvaluateFloat2(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *fl = GetNode();
  if (!fl->GetInputNode(0) || !fl->GetInputNode(1))
  {
    return;
  }

  vkString src0 = ctx.GetInputValue(fl->GetInput(0));
  vkString src1 = ctx.GetInputValue(fl->GetInput(1));

  if (src0 == src1)
  {
    // full swizzle possible
    ss << src0 << "." <<
      fl->GetInput(0)->GetInput()->GetAttr() <<
      fl->GetInput(1)->GetInput()->GetAttr();

  }
  else
  {
    // create complete new vec3
    ss << "vec2(" <<
      ctx.GetFullInputValue(fl->GetInput(0)) << ", " <<
      ctx.GetFullInputValue(fl->GetInput(1)) <<
      ")";
  }

  AssignOutput(ctx, fl->GetOutput(0), ss.str(), "vec2");
}


void vkSGNodeGL4::EvaluateFloat3(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *fl = GetNode();
  if (!fl->GetInputNode(0) || !fl->GetInputNode(1) || !fl->GetInputNode(2))
  {
    return;
  }

  vkString src0 = ctx.GetInputValue(fl->GetInput(0));
  vkString src1 = ctx.GetInputValue(fl->GetInput(1));
  vkString src2 = ctx.GetInputValue(fl->GetInput(2));

  if (src0 == src1 && src1 == src2)
  {
    // full swizzle possible
    ss << src0 << "." <<
      fl->GetInput(0)->GetInput()->GetAttr() <<
      fl->GetInput(1)->GetInput()->GetAttr() <<
      fl->GetInput(2)->GetInput()->GetAttr();

  }
  else if (src0 == src1)
  {
    // partly sizzle possible
    ss << "vec3(" <<
      src0 << "." <<
      fl->GetInput(0)->GetInput()->GetAttr() << fl->GetInput(1)->GetInput()->GetAttr() << ", " <<
      ctx.GetFullInputValue(fl->GetInput(2)) <<
      ")";
  }
  else if (src1 == src2)
  {
    // partly sizzle possible
    ss << "vec3(" <<
      src0 << "." <<
      ctx.GetFullInputValue(fl->GetInput(0)) <<
      fl->GetInput(1)->GetInput()->GetAttr() << fl->GetInput(2)->GetInput()->GetAttr() << ", " <<
      ")";
  }
  else
  {
    // create complete new vec3
    ss << "vec3(" <<
      ctx.GetFullInputValue(fl->GetInput(0)) << ", " <<
      ctx.GetFullInputValue(fl->GetInput(1)) << ", " <<
      ctx.GetFullInputValue(fl->GetInput(2)) <<
      ")";
  }

  AssignOutput(ctx, fl->GetOutput(0), ss.str(), "vec3");
}

void vkSGNodeGL4::EvaluateAddFloat(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;

  vkSGNode *add = GetNode();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";


  AssignOutput(ctx, add->GetOutput(0), ss.str(), "float");
}

void vkSGNodeGL4::EvaluateAddFloat3(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGNode *add = GetNode();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";


  AssignOutput(ctx, add->GetOutput(0), ss.str(), "vec3");
}

void vkSGNodeGL4::EvaluateSplitFloat3(vkShaderGraphCtx &ctx)
{
  vkSGNode *split = GetNode();
  vkSGNodeGL4 *inputNode = ctx.GetNode(split->GetInput(0));
  if (!inputNode)
  {
    return;
  }

  vkString exp = ctx.GetInputValue(split->GetInput(0));
  if (inputNode->IsInline())
  {
    exp = ctx.AddAssignment("vec3", exp);
  }

  ctx.SetOutputValue(split->GetOutput(0), exp);
  ctx.SetOutputValue(split->GetOutput(1), exp);
  ctx.SetOutputValue(split->GetOutput(2), exp);
}


void vkSGNodeGL4::EvaluateTexture2D(vkShaderGraphCtx &ctx)
{
  vkSGNode *txt = GetNode();
  vkString bindingName = txt->GetBindingName();
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

  ctx.SetOutputValue(txt->GetOutput(0), code);
  ctx.SetOutputValue(txt->GetOutput(1), code);
  ctx.SetOutputValue(txt->GetOutput(2), code);
  ctx.SetOutputValue(txt->GetOutput(3), code);
  ctx.SetOutputValue(txt->GetOutput(4), code);
}