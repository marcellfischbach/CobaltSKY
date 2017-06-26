
#include <graphicsgl4/shadergraph/gl4sgshadergraphctx.hh>
#include <graphicsgl4/shadergraph/gl4sgshadergraph.hh>
#include <graphicsgl4/shadergraph/gl4sgnode.hh>
#include <cobalt/graphics/shadergraph/cssgnode.hh>
#include <cobalt/graphics/shadergraph/cssginput.hh>
#include <cobalt/graphics/shadergraph/cssgoutput.hh>


csShaderGraphCtx::csShaderGraphCtx(csShaderGraphGL4 *graph)
  : m_graph(graph)
  , m_code("")
  , m_variableCounter(0)
{

}

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGNode *node)
{
  if (!node)
  {
    return 0;
  }

  csSGNodeGL4 *glNode = m_nodes[node];
  if (!glNode)
  {
    const csClass *cls = node->GetClass();
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

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGOutput *output)
{
  if (!output)
  {
    return 0;
  }

  return GetNode(output->GetNode());
}

csSGNodeGL4 *csShaderGraphCtx::GetNode(csSGInput *input)
{
  if (!input)
  {
    return 0;
  }

  return GetNode(input->GetInput());
}

void csShaderGraphCtx::Evaluate(csSGNodeGL4 *node)
{
  if (node)
  {
    node->Evaluate(*this);
  }
}

void csShaderGraphCtx::Evaluate(csSGNode *node)
{
  Evaluate(GetNode(node));
}


void csShaderGraphCtx::Evaluate(csSGOutput *output)
{
  Evaluate(GetNode(output));
}

void csShaderGraphCtx::Evaluate(csSGInput *input)
{
  Evaluate(GetNode(input));
}


void csShaderGraphCtx::EvaluateInline(csSGNodeGL4 *node)
{
  if (node)
  {
    node->EvaluateInline(*this);
  }
}

void csShaderGraphCtx::EvaluateInline(csSGNode *node)
{
  EvaluateInline(GetNode(node));
}


void csShaderGraphCtx::EvaluateInline(csSGOutput *output)
{
  EvaluateInline(GetNode(output));
}

void csShaderGraphCtx::EvaluateInline(csSGInput *input)
{
  EvaluateInline(GetNode(input));
}



csString csShaderGraphCtx::CreateCode(csSGOutput *output)
{
  csSGNodeGL4 *node = GetNode(output->GetNode());
  EvaluateInline(node);
  Evaluate(node);

  csString code = m_code;
  code += csString("\n") + GetFullOutputValue(output).c_str() + csString("\n");

  return code;
}

void csShaderGraphCtx::EvaluateInlines(std::set<csSGOutput*> outputs)
{
  for (csSGOutput *output : outputs)
  {
    EvaluateInline(output);
  }
}

void csShaderGraphCtx::GenerateCode(std::set<csSGOutput*> outputs)
{
  m_code = "";
  for (csSGOutput *output : outputs)
  {
    csSGNode *node = output->GetNode();
    Evaluate(output);
  }
}

void csShaderGraphCtx::AddExpression(const csString &expression)
{
  m_code += expression + csString(";\n");
}

csString csShaderGraphCtx::AddAssignment(const csString &type, const csString &statement)
{
  csString var = GetNextVariable();
  csString expression = type + csString(" ") + var + csString(" = ") + statement;
  AddExpression(expression);
  return var;
}


csString csShaderGraphCtx::GetNextVariable()
{
  static char buffer[64];
  m_variableCounter++;
  snprintf(buffer, 64, "v%03u", m_variableCounter);
  return csString(buffer);
}


void csShaderGraphCtx::AddBinding(csSGNode *node, const csString &variableType, const csString &variableName)
{
  ExternalBinding binding;
  binding.variableName = variableName;
  binding.node = node;
  binding.variableType = variableType;
  m_unisformBindingNames[variableName] = binding;;
}

bool csShaderGraphCtx::IsBindingApplyingFor(const csString &bindingName, csSGNode *node) const
{
  if (!node)
  {
    return false;
  }

  std::map<csString, ExternalBinding>::const_iterator it = m_unisformBindingNames.find(bindingName);
  if (it != m_unisformBindingNames.end() && it->second.node == node)
  {
    return true;
  }

  for (csSize i = 0, in = node->GetNumberOfInputs(); i < in; ++i)
  {
    csSGInput *input = node->GetInput(i);
    if (IsBindingApplyingFor(bindingName, input))
    {
      return true;
    }
  }
  return false;
}

bool csShaderGraphCtx::IsBindingApplyingFor(const csString &bindingName, csSGOutput *output) const
{
  if (!output)
  {
    return false;
  }
  return IsBindingApplyingFor(bindingName, output->GetNode());
}

bool csShaderGraphCtx::IsBindingApplyingFor(const csString &bindingName, csSGInput *input) const
{
  if (!input)
  {
    return false;
  }

  return IsBindingApplyingFor(bindingName, input->GetInput());
}

std::set<csShaderGraphCtx::ExternalBinding> csShaderGraphCtx::GetBindingsFor(std::set<csSGOutput*> outputs) const
{
  std::set<ExternalBinding> result;
  std::map<csString, ExternalBinding>::const_iterator it;
  for (it = m_unisformBindingNames.begin(); it != m_unisformBindingNames.end(); ++it)
  {
    for (csSGOutput* output : outputs)
    {
      if (IsBindingApplyingFor(it->second.variableName, output))
      {
        result.insert(it->second);
      }
    }
  }

  return result;
}

void csShaderGraphCtx::SetDefaultTextureCoordinate(const csString &defaultTextureCoordinate)
{
  m_defaultTextureCoordinate = defaultTextureCoordinate;
}

const csString &csShaderGraphCtx::GetDefaultTextureCoordinate() const
{
  return m_defaultTextureCoordinate;
}

void csShaderGraphCtx::SetOutputValue(csSGOutput *output, const csString &value)
{
  if (output)
  {
    m_outputValue[output] = value;
  }
}

bool csShaderGraphCtx::HasInputValue(csSGInput *input) const
{
  return HasOutputValue(input ? input->GetInput() : 0);
}

bool csShaderGraphCtx::HasOutputValue(csSGOutput *output) const
{
  std::map<csSGOutput *, csString>::const_iterator it = m_outputValue.find(output);
  return it != m_outputValue.end();
}

csString csShaderGraphCtx::GetOutputValue(csSGOutput *output)
{
  std::map<csSGOutput *, csString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return csString("<inval>");
  }
  return it->second;
}

csString csShaderGraphCtx::GetInputValue(csSGInput *input)
{
  return GetOutputValue(input ? input->GetInput() : 0);
}

csString csShaderGraphCtx::GetFullOutputValue(csSGOutput *output)
{
  std::map<csSGOutput *, csString>::iterator it = m_outputValue.find(output);
  if (it == m_outputValue.end())
  {
    return csString("<inval>");
  }

  csString res = it->second;

  if (output->GetAttr().length() != 0)
  {
    res = res += csString(".") + output->GetAttr();
  }
  return res;
}

csString csShaderGraphCtx::GetFullInputValue(csSGInput *input)
{
  if (input->GetInput() == 0 && input->CanInputConst())
  {
    return std::to_string(input->GetConst());
  }
  return GetFullOutputValue(input ? input->GetInput() : 0);
}
