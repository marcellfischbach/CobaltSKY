

#include <GraphicsGL4/ShaderGraph/ShaderGraphGL4.hh>
#include <iostream>
#include <sstream>


vkShaderGraphGL4::vkShaderGraphGL4()
{
  /*
  m_nodeMapping[vkSGConstFloat::GetStaticClass()] = vkSGConstFloatGL4::GetStaticClass();
  m_nodeMapping[vkSGConstFloat3::GetStaticClass()] = vkSGConstFloat3GL4::GetStaticClass();
  m_nodeMapping[vkSGFloat3::GetStaticClass()] = vkSGFloat3GL4::GetStaticClass();
  m_nodeMapping[vkSGAddFloat::GetStaticClass()] = vkSGAddFloatGL4::GetStaticClass();
  m_nodeMapping[vkSGAddFloat3::GetStaticClass()] = vkSGAddFloat3GL4::GetStaticClass();
  m_nodeMapping[vkSGSplitFloat3::GetStaticClass()] = vkSGSplitFloat3GL4::GetStaticClass();
  */
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

  return GetNode(input->GetOutput());
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


void vkShaderGraphCtx::SetOutputValue(vkSGOutput *output, const vkString &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
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
  return GetOutputValue(input ? input->GetOutput() : 0);
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
  return GetFullOutputValue(input ? input->GetOutput() : 0);
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
  const vkClass *nodeClass = m_node->GetClass();
  if (nodeClass == vkSGConstFloat::GetStaticClass())
  {
    EvaluateConstFloat(ctx);
  }
  else if (nodeClass == vkSGConstFloat3::GetStaticClass())
  {
    EvaluateConstFloat3(ctx);
  }
  else if (nodeClass == vkSGFloat3::GetStaticClass())
  {
    EvaluateFloat3(ctx);
  }
  else if (nodeClass == vkSGAddFloat::GetStaticClass())
  {
    EvaluateAddFloat3(ctx);
  }
  else if (nodeClass == vkSGAddFloat3::GetStaticClass())
  {
    EvaluateAddFloat3(ctx);
  }
  else if (nodeClass == vkSGSplitFloat3::GetStaticClass())
  {
    EvaluateSplitFloat3(ctx);
  }



  return true;
}



void vkSGNodeGL4::EvaluateConstFloat(vkShaderGraphCtx &ctx)
{
  vkSGConstFloat *fl = GetNode<vkSGConstFloat>();
  ctx.SetOutputValue(fl->GetOutput(0), std::to_string(fl->GetInput(0)->GetFloat(0)));
}

void vkSGNodeGL4::EvaluateConstFloat3(vkShaderGraphCtx &ctx)
{
  vkSGConstFloat3 *fl = GetNode<vkSGConstFloat3>();

  std::ostringstream ss;
  ss << "vec3(" <<
    std::to_string(fl->GetValueX()) << ", " <<
    std::to_string(fl->GetValueY()) << ", " <<
    std::to_string(fl->GetValueZ()) <<
    ")";

  ctx.SetOutputValue(fl->GetOutput(0), ss.str());

  ctx.SetOutputValue(fl->GetOutput(1), std::to_string(fl->GetValueX()));
  ctx.SetOutputValue(fl->GetOutput(2), std::to_string(fl->GetValueY()));
  ctx.SetOutputValue(fl->GetOutput(3), std::to_string(fl->GetValueZ()));

}

void vkSGNodeGL4::EvaluateFloat3(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGFloat3 *fl = GetNode<vkSGFloat3>();
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
      fl->GetInput(0)->GetOutput()->GetAttr() <<
      fl->GetInput(1)->GetOutput()->GetAttr() <<
      fl->GetInput(2)->GetOutput()->GetAttr();

  }
  else if (src0 == src1)
  {
    // partly sizzle possible
    ss << "vec3(" <<
      src0 << "." <<
      fl->GetInput(0)->GetOutput()->GetAttr() << fl->GetInput(1)->GetOutput()->GetAttr() << ", " <<
      ctx.GetFullInputValue(fl->GetInput(2)) <<
      ")";
  }
  else if (src1 == src2)
  {
    // partly sizzle possible
    ss << "vec3(" <<
      src0 << "." <<
      ctx.GetFullInputValue(fl->GetInput(0)) <<
      fl->GetInput(1)->GetOutput()->GetAttr() << fl->GetInput(2)->GetOutput()->GetAttr() << ", " <<
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

  vkSGAddFloat *add = GetNode<vkSGAddFloat>();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";


  AssignOutput(ctx, add->GetOutput(0), ss.str(), "float");
}

void vkSGNodeGL4::EvaluateAddFloat3(vkShaderGraphCtx &ctx)
{
  std::ostringstream ss;
  vkSGAddFloat3 *add = GetNode<vkSGAddFloat3>();
  ss << "(" << ctx.GetFullInputValue(add->GetInput(0)).c_str() << " + " <<
    ctx.GetFullInputValue(add->GetInput(1)).c_str() <<
    ")";


  AssignOutput(ctx, add->GetOutput(0), ss.str(), "vec3");
}

void vkSGNodeGL4::EvaluateSplitFloat3(vkShaderGraphCtx &ctx)
{
  vkSGSplitFloat3 *split = GetNode<vkSGSplitFloat3>();
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
